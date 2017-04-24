#include "inc/stm32f10x_gpio.h"
#include "inc/stm32f10x_tim.h"
#include "inc/stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_it.h"
#include "drivers/usart.h"
#include "drivers/printf.h"
#include "drivers/assert.h"

void Assert_Configuration() {
    //setup assert to use GPIO A11 for blinking and USART1 
    //for reporting error messages
    Assert_Init(GPIOA, GPIO_Pin_11, USE_USART1);
}

void baseNvicInit()
{
    /* Set the Vector Table base location at 0x08000000 */ 
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   

    /* 2 bit for pre-emption priority, 2 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Systick with Preemption Priority 2 and Sub Priority as 0 */ 
    NVIC_SetPriority(SysTick_IRQn,
		     NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0));
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/

void systick_init()
{
    // SysTick end of count event each 1ms with input clock equal to 72MHz
    SysTick_Config(72000);
}

