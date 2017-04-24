#include "inc/stm32f10x_i2c.h"
#include "inc/misc.h"
#include "inc/stm32f10x_rcc.h"
#include "inc/stm32f10x_gpio.h"
#include "stm32f10x_it.h"
#include "i2c.h"
#include "printf.h"

enum I2CModes {
    I2C_READ,
    I2C_WRITE,
    I2C_WRITE_READ,
    I2C_FINISHED,
};

enum I2CState {
    START_WRITTEN,
    ADDRESS_WRITTEN,
    HANDLING_DATA,
    STOP_WRITTEN,
};


struct I2C_Data {
    enum I2CState state;
    u8 I2C_Buffer_Tx[4];
    u8 I2C_Buffer_Rx[4];
    u8 I2C_Tx_Idx;
    u8 I2C_Rx_Idx;
    u8 I2C_Tx_Size;
    u8 I2C_Rx_Size;
    u8 I2CMode;
    u8 I2CError;
    u32 I2CErrorReason;
    // counter to detect i2c interrupt storm (flooding)
    u32 i2cSafetyCounter;
    u8 curI2CAddr;
    int curI2CSpeed;
    FunctionalState curI2CIsRemapped;
};

void printfI2CDbg();
void I2C_EV_IRQHandler(I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data);
void I2C_ER_IRQHandler(I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data);

u8 handleI2CxErrors(I2C_TypeDef* I2Cx, volatile struct I2C_Data* I2Cx_Data);
u8 I2CSendBytes(u8 *data, u8 size, u8 addr, I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data);
u8 I2CReadBytes(u8 size, u8 addr,I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data);
u8 I2CWriteReadBytes(u8 *txdata, u8 txsize, u8 rxsize, u8 addr, I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data);
u8 I2CxOperationFinished(I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data);
void setupI2Cx(u16 address, int speed, I2C_TypeDef* I2Cx, FunctionalState remapped);

volatile struct I2C_Data I2C1_Data;
volatile struct I2C_Data I2C2_Data;

void setupI2Cx(u16 address, int speed, I2C_TypeDef* I2Cx, FunctionalState remapped) {
    volatile int wait;
    volatile struct I2C_Data *I2Cx_Data;
    u8 EV_IRQChannel;
    u8 ER_IRQChannel;
    u16 GPIOs;
    GPIO_TypeDef* GPIO_Block = GPIOB;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    if(I2Cx == I2C1)
    {
	I2Cx_Data = &I2C1_Data;
	EV_IRQChannel = I2C1_EV_IRQn;
	ER_IRQChannel = I2C1_ER_IRQn;
	
	if(remapped)
	{    
	    GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
	    GPIOs = GPIO_Pin_8 | GPIO_Pin_9;
	}
	else
	{    
	    GPIOs = GPIO_Pin_6 | GPIO_Pin_7;
	}
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    }
    else
    {
	I2Cx_Data = &I2C2_Data;
	EV_IRQChannel = I2C2_EV_IRQn;
	ER_IRQChannel = I2C2_ER_IRQn;
	
	GPIOs = GPIO_Pin_10 | GPIO_Pin_11;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    }

    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure and enable I2C2 interrupt ------------------------------------*/
    NVIC_InitStructure.NVIC_IRQChannel = EV_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = ER_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
    
    I2C_DeInit(I2Cx);

    //configure as in floating, before (RE) initalizing.
    //this works around the buggy i2c controller that 
    //does not release the line in case of error
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIOs;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_Block, &GPIO_InitStructure);  

    wait = 10000;
    while(wait--)
	;

    //Configure I2C2 Pins, SDA and SCL
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIOs;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIO_Block, &GPIO_InitStructure);  

    I2C_InitTypeDef  I2C_InitStructure;
    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_OwnAddress1 = address;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = speed;

    if(speed <= 100000)
    {
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; // for < 100 kHz
    } else
    {
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
    }
    I2C_Init(I2Cx, &I2C_InitStructure);

    //Enable I2C2
    I2C_Cmd(I2Cx, ENABLE);

    // Disable PEC for I2C2
    I2C_CalculatePEC(I2Cx, DISABLE);

    //setup mode correctly
    I2Cx_Data->I2CMode = I2C_FINISHED;
    I2Cx_Data->curI2CAddr = address;
    I2Cx_Data->curI2CSpeed = speed;
    I2Cx_Data->curI2CIsRemapped = remapped;
    I2Cx_Data->I2C_Tx_Idx = 0;
    I2Cx_Data->I2C_Rx_Idx = 0;
    I2Cx_Data->I2C_Tx_Size = 0;
    I2Cx_Data->I2C_Rx_Size = 0;
    I2Cx_Data->I2CError = 0;
    I2Cx_Data->I2CErrorReason = 0;
    I2Cx_Data->i2cSafetyCounter = 0;

    // Enable I2C1 and I2C2 event and buffer interrupt
    I2C_ITConfig(I2Cx, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE); 
}

void setupI2C2(u16 address, int speed) {
    //i2c2 can not be remapped
    setupI2Cx(address, speed, I2C2, DISABLE);
}

void setupI2C1(u16 address, int speed, FunctionalState remapped) {
    setupI2Cx(address, speed, I2C1, remapped);
}

void I2CSendBytesBlocking(u8 *data, u8 size, u8 addr, I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data)
{
    u8 error = 0;
    error = I2CSendBytes(data, size, addr, I2Cx, I2Cx_Data);
    
    while(!I2C2OperationFinished() || error)
    {
	if(handleI2C2Errors() || error)
	{
	    error = I2C2SendBytes(data, size, addr);
	    printf("e");
	}
    }
}

void I2C2SendBytesBlocking(u8 *data, u8 size, u8 addr)
{
    I2CSendBytesBlocking(data, size, addr, I2C2, &I2C2_Data);
}


u8 handleI2CxErrors(I2C_TypeDef* I2Cx, volatile struct I2C_Data* I2Cx_Data)
{
    u8 hasError = 0;

    if(I2Cx_Data->I2CError == 2) {
	printf("Bus Amok\n");
    }
    //handle I2C errors
    if(I2Cx_Data->I2CError) {
	switch(I2Cx_Data->I2CErrorReason) {
	case I2C_FLAG_ARLO:
	    printf("ARLO");
	    break;
	case I2C_FLAG_BERR:
	    printf("BERR");
	    I2C_SoftwareResetCmd(I2Cx, DISABLE);
	    setupI2Cx(I2Cx_Data->curI2CAddr, I2Cx_Data->curI2CSpeed, I2Cx, I2Cx_Data->curI2CIsRemapped);
	    break;
	case I2C_FLAG_OVR:
	    printf("OVR");
	    break;
	case I2C_FLAG_AF:
	    printf("AF");
	    break;	
	default:
	    I2C_SoftwareResetCmd(I2Cx, ENABLE);
	    int wait = 1000;
	    while(wait--)
		;
	    I2C_SoftwareResetCmd(I2Cx, DISABLE);
	    setupI2Cx(I2Cx_Data->curI2CAddr, I2Cx_Data->curI2CSpeed, I2Cx, I2Cx_Data->curI2CIsRemapped);
	    printf("Unknown %lu", I2Cx_Data->I2CErrorReason);
	}
	
	I2Cx_Data->I2CMode = I2C_FINISHED;
	
	I2Cx_Data->I2CError = 0;
	hasError = 1;    
    }

    return hasError;

}


u8 handleI2C1Errors()
{
    return handleI2CxErrors(I2C1, &I2C1_Data);
}

u8 handleI2C2Errors()
{
    return handleI2CxErrors(I2C2, &I2C2_Data);
}


// define I2C register
#define I2C_SMBALERT (1<<15)
#define I2C_TIMEOUT (1<<14)
#define I2C_PECERR (1<<12)
#define I2C_OVR (1<<11)
#define I2C_AF (1<<10)
#define I2C_ARLO (1<<9)
#define I2C_BERR (1<<8)
#define I2C_TxE (1<<7)
#define I2C_RxNE (1<<6)
#define I2C_STOPF (1<<4)
#define I2C_ADD10 (1<<3)
#define I2C_BTF (1<<2)
#define I2C_ADDR (1<<1)
#define I2C_SB (1<<0)
#define I2C_DUALF (1<<7)
#define I2C_SMBHOST (1<<6)
#define I2C_SMBDEFAULT (1<<5)
#define I2C_GENCALL (1<<4)
#define I2C_TRA (1<<2)
#define I2C_BUSY (1<<1)
#define I2C_MSL (1<<0)

/*******************************************************************************
* Function Name  : I2C2_ER_IRQHandler
* Description    : This function handles I2C2 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_ER_IRQHandler(void)
{
  I2C_ER_IRQHandler(I2C2, &I2C2_Data);
}

struct i2cDebug {
    u16 sr1;
    u16 sr2;
    u8 state;
    u8 mode;
    u8 idx;
    u8 size;
    u16 cnt;
    u8 error;
};

#ifdef I2C_DEBUG
#define dbgBufferSize 20
volatile u16 dbgWrite = 0;
volatile u16 dbgRead = 0;
volatile struct i2cDebug dbgBuffer[dbgBufferSize];

void printfI2CDbg() {
    while(dbgWrite != dbgRead) {
	volatile struct i2cDebug *dbg = &(dbgBuffer[dbgRead]);
	dbgRead = (dbgRead +1 ) % dbgBufferSize;
	printf("cnt %hu, SR1 %hu, SR2 %hu, S %hu, M %hu, idx %hu, size %hu, error %hu\n", dbg->cnt, dbg->sr1, dbg->sr2, dbg->state, dbg->mode, dbg->idx, dbg->size, dbg->error);
    }
}
#endif

void I2C_EV_IRQHandler(I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data)
{
    u16 sr1 = I2Cx->SR1;
    u16 sr2 = I2Cx->SR2;
    static u16 cnt = 0;

#ifdef I2C_DEBUG
    //flush debug buffer
    if(I2Cx_Data->i2cSafetyCounter == 0)
	dbgWrite = dbgRead;
#endif

    ++(I2Cx_Data->i2cSafetyCounter);

    // interrupt storm detected, disable irq
    if(I2Cx_Data->i2cSafetyCounter > 120) {
	I2Cx_Data->I2CError = 2;
	I2Cx_Data->I2CErrorReason = I2C_FLAG_BERR;
	I2C_SoftwareResetCmd(I2Cx, ENABLE);
	I2C_ITConfig(I2Cx, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, DISABLE);
    }
    
#ifdef I2C_DEBUG
    vu8 nextRxWritePointer = (dbgWrite + 1) % dbgBufferSize;

    if(nextRxWritePointer != dbgRead) {
	volatile struct i2cDebug *dbg = &(dbgBuffer[dbgWrite]);
	dbg->sr1 = sr1;
	dbg->sr2 = sr2;
	dbg->state = I2Cx_Data->state;
	dbg->mode = I2Cx_Data->I2CMode;
	dbg->idx = I2Cx_Data->I2C_Rx_Idx;
	dbg->size = I2Cx_Data->I2C_Rx_Size;
	dbg->cnt = cnt;
	dbg->error = I2Cx_Data->I2CError;
	dbgWrite = nextRxWritePointer;
    }
#endif
    
    cnt++;
    
    switch(I2Cx_Data->state) {
	case START_WRITTEN:
	    if(sr1& I2C_SB) {
		//we are master
		//write adress of slave
		if(I2Cx_Data->I2CMode == I2C_READ) {
		    // Send I2C2 slave Address for write
		    I2C_Send7bitAddress(I2Cx, I2Cx_Data->curI2CAddr, I2C_Direction_Receiver);
		} else {//I2Cx_Data->I2CMode == I2C_WRITE || I2Cx_Data->I2CMode == I2C_WRITE_READ
		    if(I2Cx_Data->I2CMode != I2C_FINISHED) { 
			// Send I2C2 slave Address for write
			I2C_Send7bitAddress(I2Cx, I2Cx_Data->curI2CAddr, I2C_Direction_Transmitter);
		    }
		}
		//enable buf it, so we get an interrupt if TxE or RxNE
		I2C_ITConfig(I2Cx, I2C_IT_BUF, ENABLE);

		I2Cx_Data->state = ADDRESS_WRITTEN;
	    }
	    break;
	case ADDRESS_WRITTEN:
	    //advance state for debug purposes
	    if(sr1 & I2C_ADDR) {
		I2Cx_Data->state = HANDLING_DATA;		
	    }
	    //no break here by purpose
	case HANDLING_DATA:
	    //this basicly means, we are not in transmission
	    //mode, but the chip is waiting for data to transmit
	    //as we had this error, we are going to handle this case
	    if(sr1 & I2C_TxE && !(sr2 & I2C_TRA)) {
		I2C_SendData(I2Cx, 0);		
	    }
	    
	    if(sr2 & I2C_TRA) {
		//Transmitter
		if(sr1 & I2C_TxE || sr1 & I2C_BTF) {
		    if(I2Cx_Data->I2C_Tx_Idx < I2Cx_Data->I2C_Tx_Size) {
			/* Transmit buffer data */
			I2C_SendData(I2Cx, I2Cx_Data->I2C_Buffer_Tx[I2Cx_Data->I2C_Tx_Idx]);
			I2Cx_Data->I2C_Tx_Idx++;
		    } else {
			if(I2Cx_Data->I2CMode == I2C_WRITE) {
                            /* Disable I2C1 BUF interrupts */
                            I2C_ITConfig(I2Cx, I2C_IT_BUF, DISABLE);
                            // Send STOP Condition
                            I2C_GenerateSTOP(I2Cx, ENABLE);

                            //write some data to DR, so that BTF is never set
                            //note this data is NOT sended
                            I2C_SendData(I2Cx, 0);

                            I2Cx_Data->I2CMode = I2C_FINISHED;
			} else {
			    if(I2Cx_Data->I2CMode == I2C_WRITE_READ) {
				//Write phase finished, switch to read
				I2Cx_Data->I2CMode = I2C_READ;

				/* Disable I2C1 BUF interrupts */
				I2C_ITConfig(I2Cx, I2C_IT_BUF, DISABLE);
				
				/* Send I2C1 START condition */
				I2C_GenerateSTART(I2Cx, ENABLE);	  

				//set state back to start written
				I2Cx_Data->state = START_WRITTEN;
				
				//write some data to DR, so that BTF is never set
				//note this data is NOT sended
				I2C_SendData(I2Cx, 0);
			    } else {
				//handle TxE or BTF in weird state
                                //note we should never end up here !
				I2C_SendData(I2Cx, 0);
			    }
			}
		    }
		}
	    } else {
		//receiver
		//for one byte receiving ack needs to be disabled just after writing the address
		if(I2Cx_Data->I2C_Rx_Size == 1 && sr1 & I2C_ADDR) {
		    // Disable Acknowledgement
		    I2C_AcknowledgeConfig(I2Cx, DISABLE);

		    // Send STOP Condition
		    I2C_GenerateSTOP(I2Cx, ENABLE);
		}

		//read data if rx is not empty
		if(sr1 & I2C_RxNE || sr1 & I2C_BTF) {
		    if(I2Cx_Data->I2C_Rx_Idx < I2Cx_Data->I2C_Rx_Size) {
			I2Cx_Data->I2C_Buffer_Rx[I2Cx_Data->I2C_Rx_Idx] = I2C_ReceiveData(I2Cx);
			I2Cx_Data->I2C_Rx_Idx++;
			//set mode to finish if we got all bytes
			if(I2Cx_Data->I2C_Rx_Idx >= I2Cx_Data->I2C_Rx_Size)
			    I2Cx_Data->I2CMode = I2C_FINISHED;	
		    } else {
			//clear RxNE
			I2C_ReceiveData(I2Cx);
		    }
		    
		    //test if next byte is last one and disable ack
		    if(I2Cx_Data->I2C_Rx_Idx + 1 == I2Cx_Data->I2C_Rx_Size) {
			// Disable Acknowledgement
			I2C_AcknowledgeConfig(I2Cx, DISABLE);
			// Send STOP Condition
			I2C_GenerateSTOP(I2Cx, ENABLE);
		    }
		}
	    }
	    break;
	case STOP_WRITTEN:
	    break;
    }    
}


void I2C_ER_IRQHandler(I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data) {
  I2Cx_Data->I2CError = 1;
  u16 sr2 = I2Cx->SR2;
  u16 cr1 = I2Cx->CR1;
  
  //we got an error, be sure we don't stop
  //later by an orphan start condition
  I2C_GenerateSTART(I2Cx, DISABLE);
  
  (I2Cx_Data->i2cSafetyCounter)++;

  if(I2Cx_Data->i2cSafetyCounter > 120) {
      I2Cx_Data->I2CError = 2;
      I2Cx_Data->I2CErrorReason = I2C_FLAG_BERR;
      I2C_SoftwareResetCmd(I2Cx, ENABLE);
      I2C_ITConfig(I2Cx, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, DISABLE);
  }
  
#ifdef I2C_DEBUG
  vu8 nextRxWritePointer = (dbgWrite + 1) % dbgBufferSize;
  //if(nextRxWritePointer != dbgRead) {
	volatile struct i2cDebug *dbg = &(dbgBuffer[dbgWrite]);
	dbg->sr1 = cr1;
	dbg->sr2 = sr2;
	dbg->state = I2Cx_Data->state;
	dbg->mode = I2Cx_Data->I2CMode;
	dbg->idx = I2Cx_Data->I2C_Rx_Idx;
	dbg->size = I2Cx_Data->I2C_Rx_Size;
	dbg->cnt = 666;
	dbg->error = I2Cx_Data->I2CError;
	dbgWrite = nextRxWritePointer;
    //}
#endif
	
  //Acknowledge failure
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)) {
    I2Cx_Data->I2CErrorReason = I2C_FLAG_AF;
  
    //Note, master only error handling
    if(!(cr1 & (1<<9) || cr1 & (1<<8)) && sr2 & I2C_BUSY)
	I2C_GenerateSTOP(I2Cx, ENABLE);

    /* Clear AF flag */
    I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
  }

  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ARLO)) {
    I2Cx_Data->I2CErrorReason = I2C_FLAG_ARLO;
    // Send STOP Condition
    if(!(cr1 & (1<<9) || cr1 & (1<<8)) && sr2 & I2C_BUSY)
	I2C_GenerateSTOP(I2Cx, ENABLE);

    I2C_ClearFlag(I2Cx, I2C_FLAG_ARLO);
  }

  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TIMEOUT)) {
      //I2Cx_Data->I2CErrorReason = I2C_FLAG_TIMEOUT;
    // Send STOP Condition
    if(!(cr1 & (1<<9) || cr1 & (1<<8)) && sr2 & I2C_BUSY)
	I2C_GenerateSTOP(I2Cx, ENABLE);

    I2C_ClearFlag(I2Cx, I2C_FLAG_TIMEOUT);
  }
  
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BERR)) {
    I2Cx_Data->I2CErrorReason = I2C_FLAG_BERR;
    I2C_SoftwareResetCmd(I2Cx, ENABLE);

    I2C_ClearFlag(I2Cx, I2C_FLAG_BERR);
  }
  
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_OVR)) {
    I2Cx_Data->I2CErrorReason = I2C_FLAG_OVR;
    // Send STOP Condition
    I2C_GenerateSTOP(I2Cx, ENABLE);

    I2C_ClearFlag(I2Cx, I2C_FLAG_OVR);
  }
}

/*******************************************************************************
* Function Name  : I2C1_EV_IRQHandler
* Description    : This function handles I2C1 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void I2C1_EV_IRQHandler(void)
{
  I2C_EV_IRQHandler(I2C1, &I2C1_Data);
}

/*******************************************************************************
* Function Name  : I2C1_ER_IRQHandler
* Description    : This function handles I2C1 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_ER_IRQHandler(void)
{
  I2C_ER_IRQHandler(I2C1, &I2C1_Data);
}

/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{ 
  I2C_EV_IRQHandler(I2C2, &I2C2_Data);
}

u8 I2CxOperationFinished(I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data)
{
#ifdef I2C_DEBUG
    if(I2Cx_Data->i2cSafetyCounter >= 120) {
	printfI2CDbg();
    }
#endif
    return I2Cx_Data->I2CMode == I2C_FINISHED && !I2C_GetFlagStatus(I2Cx, I2C_FLAG_MSL);
}


u8 I2C1OperationFinished() {
    return I2CxOperationFinished(I2C1, &I2C1_Data);
};

u8 I2C2OperationFinished() {
    return I2CxOperationFinished(I2C2, &I2C2_Data);
};

u8 I2CSendBytes(u8 *data, u8 size, u8 addr, I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data) {
    
  if(I2Cx_Data->I2CError) {
    printf("I2C in error status \n");
    return 1;
  }
  
  if(I2Cx_Data->I2CMode != I2C_FINISHED) {
    printf("I2C still active \n");
    return 1;
  }
  
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_MSL)) {
//     printf("BAD, I am still a master \n");
    return 1;
  }

#ifdef I2C_DEBUG
    if(I2Cx_Data->i2cSafetyCounter >= 120) {
	printfI2CDbg();
    }
#endif

  I2Cx_Data->i2cSafetyCounter = 0;

  //setup interrupt handler state machine
  I2Cx_Data->I2C_Tx_Idx = 0;
  I2Cx_Data->I2C_Tx_Size = size;
  I2Cx_Data->curI2CAddr = addr;
  I2Cx_Data->I2CMode = I2C_WRITE;
  int i;
  I2Cx_Data->state = START_WRITTEN;
  for(i = 0; i < size; i++) {
    I2Cx_Data->I2C_Buffer_Tx[i] = data[i];
  }

  //disable orphaned stop conditions
  I2C_GenerateSTOP(I2Cx, DISABLE);

  // Enable Acknowledgement
  I2C_AcknowledgeConfig(I2Cx, ENABLE);
	
  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2Cx, ENABLE);
  
  return 0;
}

u8 I2CReadBytes(u8 size, u8 addr,I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data) {
  if(I2Cx_Data->I2CError) {
    //printf("I2C1 in error status \n");
    return 1;
  }
  
  if(I2Cx_Data->I2CMode != I2C_FINISHED) {
    printf("I2C1 still active \n");
    return 1;
  }
  
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_MSL)) {
//     printf("BAD, I am still a master \n");
    return 1;
  }

#ifdef I2C_DEBUG
    if(I2Cx_Data->i2cSafetyCounter >= 120) {
	printfI2CDbg();
    }
#endif

  I2Cx_Data->i2cSafetyCounter = 0;

  //setup interrupt handler state machine
  I2Cx_Data->I2C_Rx_Idx = 0;
  I2Cx_Data->I2C_Rx_Size = size;
  I2Cx_Data->curI2CAddr = addr;
  I2Cx_Data->I2CMode = I2C_READ;
  I2Cx_Data->state = START_WRITTEN;

   //disable orphaned stop conditions
  I2C_GenerateSTOP(I2Cx, DISABLE);

  // Enable Acknowledgement
  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  
  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2Cx, ENABLE);

  return 0;
}

u8 I2CWriteReadBytes(u8 *txdata, u8 txsize, u8 rxsize, u8 addr, I2C_TypeDef* I2Cx, volatile struct I2C_Data *I2Cx_Data) {
  if(I2Cx_Data->I2CError) {
    //printf("I2C1 in error status \n");
    return 1;
  }
  
  if(I2Cx_Data->I2CMode != I2C_FINISHED) {
    printf("I2C1 still active \n");
    return 1;
  }
  
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_MSL)) {
//     printf("BAD, I am still a master \n");
    return 1;
  }

#ifdef I2C_DEBUG
    if(I2Cx_Data->i2cSafetyCounter >= 120) {
	printI2CDbg();
    }
#endif

  I2Cx_Data->i2cSafetyCounter = 0;

  //setup interrupt handler state machine
  I2Cx_Data->I2C_Tx_Idx = 0;
  I2Cx_Data->I2C_Tx_Size = txsize;
  I2Cx_Data->I2C_Rx_Idx = 0;
  I2Cx_Data->I2C_Rx_Size = rxsize;
  I2Cx_Data->curI2CAddr = addr;
  I2Cx_Data->I2CMode = I2C_WRITE_READ;
  I2Cx_Data->state = START_WRITTEN;
  
  int i;
  
  for(i = 0; i < txsize; i++) {
    I2Cx_Data->I2C_Buffer_Tx[i] = txdata[i];
  }

  //disable orphaned stop conditions
  I2C_GenerateSTOP(I2Cx, DISABLE);

  // Enable Acknowledgement
  I2C_AcknowledgeConfig(I2Cx, ENABLE);

  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2Cx, ENABLE);

  return 0;
}


u8 I2C1SendBytes(u8 *data, u8 size, u8 addr) {
  return I2CSendBytes(data, size, addr, I2C1, &I2C1_Data);
}

u8 I2C1ReadBytes(u8 size, u8 addr) {
  return I2CReadBytes(size, addr, I2C1, &I2C1_Data);
}

u8 I2C1WriteReadBytes(u8 *txdata, u8 txsize, u8 rxsize, u8 addr) {
  return I2CWriteReadBytes(txdata, txsize, rxsize, addr, I2C1, &I2C1_Data);
}

u8 I2C2SendBytes(u8 *data, u8 size, u8 addr) {
  return I2CSendBytes(data, size, addr, I2C2, &I2C2_Data);
}

u8 I2C2ReadBytes(u8 size, u8 addr) {
  return I2CReadBytes(size, addr, I2C2, &I2C2_Data);
}

u8 I2C2WriteReadBytes(u8 *txdata, u8 txsize, u8 rxsize, u8 addr) {
  return I2CWriteReadBytes(txdata, txsize, rxsize, addr, I2C2, &I2C2_Data);
}
