#include "printf.h"
#include "inc/stm32f10x_gpio.h"
#include "inc/stm32f10x_rcc.h"
#include "usart.h"
#include "assert.h"

GPIO_TypeDef* assertGpio;
uint16_t assertGpioPin;
enum ASSERT_UASRT assertUsart;


void Assert_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, enum ASSERT_UASRT used_usart)
{
    assertGpio = GPIOx;
    assertGpioPin = GPIO_Pin;
    assertUsart = used_usart;
}


/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    __disable_irq();
    switch(assertUsart)
    {
	case USE_USART1:
	    USART1_DeInit();
	    //do not use interrupts in assert case
	    USART1_Init(USART_POLL);
	    printf_setSendFunction(&USART1_SendData);
	    break;
	case USE_USART2:
	    USART2_DeInit();
	    //do not use interrupts in assert case
	    USART2_Init(USART_POLL);
	    printf_setSendFunction(&USART2_SendData);
	    break;
    }
    
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    printf("Wrong parameters value: file %s on line %lu\n", file, line);

    GPIO_InitTypeDef GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
    
    // Configure PC.12 as output push-pull (LED)
    GPIO_WriteBit(assertGpio,assertGpioPin,Bit_SET);
    GPIO_InitStructure.GPIO_Pin =  assertGpioPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(assertGpio, &GPIO_InitStructure);
    
    volatile int delay;
    int waittime = 500000;
    
    while(1)
    {  
	GPIO_SetBits(assertGpio, assertGpioPin);
	delay = waittime;
	while(delay) {
	    delay--;
	}
	
	GPIO_ResetBits(assertGpio, assertGpioPin);
	delay = waittime;
	while(delay) {
	    delay--;
	}
    }
}

#define printDebug(debugString) \
    switch(assertUsart) \
    {\
	case USE_USART1:\
	    USART1_DeInit();\
	    USART1_Init(USART_POLL); \
	    printf_setSendFunction(&USART1_SendData); \
	    break;\
	case USE_USART2:\
	    USART2_DeInit();\
	    USART2_Init(USART_POLL);\
	    printf_setSendFunction(&USART2_SendData); \
	    break;\
    }\
    printf(#debugString "\n");

void NMI_Handler(void)
{
    printDebug(NMIException)
    assert_failed((uint8_t *)__FILE__, __LINE__);
}

void HardFault_Handler(void)
{
    printDebug(HardFaultException)
    assert_failed((uint8_t *)__FILE__, __LINE__);
}

void MemManage_Handler(void)
{
    printDebug(MemManageException)
    assert_failed((uint8_t *)__FILE__, __LINE__);
}

void BusFault_Handler(void)
{
    printDebug(BusFaultException)
    assert_failed((uint8_t *)__FILE__, __LINE__);
}

void UsageFault_Handler(void)
{
    printDebug(UsageFaultException)
    assert_failed((uint8_t *)__FILE__, __LINE__);
}


