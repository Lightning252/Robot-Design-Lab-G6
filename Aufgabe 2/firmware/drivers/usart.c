#include "usart.h"
#include "inc/stm32f10x_usart.h"
#include "inc/stm32f10x_rcc.h"
#include "inc/stm32f10x_gpio.h"

#include "stm32f10x_it.h"

#define USART_BUFFER_SIZE 256

//internal helper functions, 
//user space does not need to know 
//about these
struct USART_Data {
  uint8_t RxBuffer[USART_BUFFER_SIZE];
  uint16_t RxWritePointer;
  uint16_t RxReadPointer;
  
  uint8_t TxBuffer[USART_BUFFER_SIZE];
  uint16_t TxWritePointer;
  uint16_t TxReadPointer;

  uint8_t RxBufferFullError;

  FunctionalState usesInterrupts;
};

signed int USARTx_SendData(USART_TypeDef* USARTx, volatile struct USART_Data *usart_data,const uint8_t *data, const unsigned int size);
signed int USARTx_SeekData(USART_TypeDef* USARTx, volatile struct USART_Data *usart_data, uint8_t *data, const unsigned int size);
signed int USARTx_GetData(USART_TypeDef* USARTx, volatile struct USART_Data *usart_data, uint8_t *buffer, const unsigned int buffer_length);
void USART_IRQHandler(USART_TypeDef* USARTx, volatile struct USART_Data *data);

volatile struct USART_Data USART1_Data;
volatile struct USART_Data USART2_Data;

void USART1_Init(enum USART_MODE mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    assert_param(mode == USART_USE_INTERRUPTS || mode == USART_POLL);

    //get default GPIO config
    GPIO_StructInit(&GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // Configure USART1 Tx (PA09) as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure USART1 Rx (PA10) as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    USART1_Data.RxWritePointer = 0;
    USART1_Data.RxReadPointer = 0;
    
    USART1_Data.TxWritePointer = 0;
    USART1_Data.TxReadPointer = 0;
    USART1_Data.usesInterrupts = mode == USART_USE_INTERRUPTS;

    if(mode == USART_USE_INTERRUPTS)
    {
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    }

    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    /* Configure USART1 */
    USART_Init(USART1, &USART_InitStructure);

    /* Enable USART1 Receive and Transmit interrupts */
    USART_ITConfig(USART1, USART_IT_RXNE, mode == USART_USE_INTERRUPTS);
    USART_ITConfig(USART1, USART_IT_TXE, mode == USART_USE_INTERRUPTS);

    /* Enable the USART1 */
    USART_Cmd(USART1, ENABLE);
}

void USART1_DeInit(void )
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Disable USART1 Receive and Transmit interrupts
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

    // Enable the USART1
    USART_Cmd(USART1, DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
}

signed int USART1_SendData(const unsigned char *data, const unsigned int size) {
    return USARTx_SendData(USART1, &USART1_Data, data, size);
}

signed int USART1_SeekData(unsigned char* buffer, const unsigned int buffer_length)
{
    return USARTx_SeekData(USART1, &USART1_Data, buffer, buffer_length);
}

signed int USART1_GetData (unsigned char *buffer, const unsigned int buffer_length) {
    return USARTx_GetData(USART1, &USART1_Data, buffer, buffer_length);
}

void USART1_IRQHandler(void)
{ 
    USART_IRQHandler(USART1, &USART1_Data);
}


void USART2_Init(enum USART_MODE mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    assert_param(mode == USART_USE_INTERRUPTS || mode == USART_POLL);

    //get default GPIO config
    GPIO_StructInit(&GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // Configure USART2 Tx (PA02 as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure USART2 Rx (PA10) as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    USART2_Data.RxWritePointer = 0;
    USART2_Data.RxReadPointer = 0;
    
    USART2_Data.TxWritePointer = 0;
    USART2_Data.TxReadPointer = 0;
    USART2_Data.usesInterrupts = mode == USART_USE_INTERRUPTS;

    if(mode == USART_USE_INTERRUPTS)
    {
        NVIC_InitTypeDef NVIC_InitStructure;
        
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }

    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = 921600; //115200 * 2;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    /* Configure USART2 */
    USART_Init(USART2, &USART_InitStructure);

    /* Enable USART2 Receive and Transmit interrupts */
    USART_ITConfig(USART2, USART_IT_RXNE, mode == USART_USE_INTERRUPTS);
    USART_ITConfig(USART2, USART_IT_TXE, mode == USART_USE_INTERRUPTS);

    /* Enable the USART1 */
    USART_Cmd(USART2, ENABLE);
}

void USART2_DeInit(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Disable USART1 Receive and Transmit interrupts
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);

    // Enable the USART1
    USART_Cmd(USART2, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
}

signed int USART2_SendData(const unsigned char *data, const unsigned int size) {
  return USARTx_SendData(USART2, &USART2_Data, data, size);
}

signed int USART2_GetData (unsigned char *buffer, const unsigned int buffer_length) {
  return USARTx_GetData(USART2, &USART2_Data, buffer, buffer_length);
}

void USART2_IRQHandler(void)
{ 
  USART_IRQHandler(USART2, &USART2_Data);
}

signed int USART2_SeekData(unsigned char* buffer, const unsigned int buffer_length)
{
    return USARTx_SeekData(USART2, &USART2_Data, buffer, buffer_length);
}

/**
 * returns the amount of bytes sent.
 */
signed int USARTx_SendData(USART_TypeDef* USARTx, volatile struct USART_Data *usart_data,const unsigned char *buffer, const unsigned int count) {
    if(!usart_data->usesInterrupts)
    {
	uint32_t pos = 0;

	for(pos = 0; pos < count; pos++)
	{
	    USART_SendData(USARTx, buffer[pos]);
	
	    //Wait until data is send
	    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
	    {
	    }
	}
	return count;
    }
    
    uint32_t size = 0;

    uint16_t nextPtr = (usart_data->TxWritePointer + 1) % USART_BUFFER_SIZE; 
    while(size < count && nextPtr != usart_data->TxReadPointer) {
	usart_data->TxBuffer[usart_data->TxWritePointer] = buffer[size];
	usart_data->TxWritePointer = (usart_data->TxWritePointer + 1) % USART_BUFFER_SIZE;
	size++;
	nextPtr = (usart_data->TxWritePointer + 1) % USART_BUFFER_SIZE;
    }

    //Enable the USARTx Transmit interrupt
    USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
    
    return size;
}

/**
 * Fetches Data from the receive queue.
 * returns bytes fetched from rx queue
 */
signed int USARTx_GetData(USART_TypeDef* USARTx, volatile struct USART_Data *usart_data, unsigned char *buffer, const unsigned int buffer_length) {
    uint32_t counter = 0;
    while(counter < buffer_length && usart_data->RxWritePointer != usart_data->RxReadPointer) {
	buffer[counter] = usart_data->RxBuffer[usart_data->RxReadPointer];
	counter++;
	usart_data->RxReadPointer = (usart_data->RxReadPointer + 1) % USART_BUFFER_SIZE;
    }
    
    if(usart_data->RxWritePointer == usart_data->RxReadPointer && usart_data->RxBufferFullError)
    {
	//clear error
	usart_data->RxBufferFullError = 0;
	//reenable receive interrupt
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	//inform user space that we had an error
	return -1;
    }
    
    return counter;
}

signed int USARTx_SeekData(USART_TypeDef* USARTx, volatile struct USART_Data *usart_data, uint8_t *data, const unsigned int size)
{
    uint32_t counter = 0;
    uint16_t tmpReadPointer = usart_data->RxReadPointer;
    while(counter < size && usart_data->RxWritePointer != tmpReadPointer) {
	data[counter] = usart_data->RxBuffer[tmpReadPointer];
	counter++;
	tmpReadPointer = (tmpReadPointer + 1) % USART_BUFFER_SIZE;
    }
    
    if(usart_data->RxWritePointer == usart_data->RxReadPointer && usart_data->RxBufferFullError)
    {
	//clear error
	usart_data->RxBufferFullError = 0;
	//reenable receive interrupt
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	//inform user space that we had an error
	return -1;
    }
    
    return counter;    
}


/*******************************************************************************
* Function Name  : USARTx_IRQHandler
* Description    : This function handles USARTx global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART_IRQHandler(USART_TypeDef* USARTx,volatile struct USART_Data *data)
{
  if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
  {
    volatile uint8_t nextRxWritePointer = (data->RxWritePointer + 1) % USART_BUFFER_SIZE;

    //ringbuffer is full :-((
    if(nextRxWritePointer == data->RxReadPointer) {
      //set error flag and disable receiving
      data->RxBufferFullError = 1;
      //Disable the USARTx Receive interrupt
      USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);
    }

    // Read one byte from the receive data register
    data->RxBuffer[data->RxWritePointer] = USART_ReceiveData(USARTx);      
    data->RxWritePointer = nextRxWritePointer;
  }
  
  if(USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
  {
    //ringbuffer empty, nothing more to send
    if(data->TxReadPointer == data->TxWritePointer)
    {
      //Disable the USARTx Transmit interrupt
      USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
    } else {
      // Write one byte to the transmit data register
      USART_SendData(USARTx, data->TxBuffer[data->TxReadPointer]);                    
      data->TxReadPointer = (data->TxReadPointer + 1) % USART_BUFFER_SIZE;
    }
  }
}
