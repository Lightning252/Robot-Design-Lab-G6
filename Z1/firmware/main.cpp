#include "drivers/printf.h"
#include "drivers/usart.h"
#include <stdlib.h>
#include "stm32_specific.h"
#include <stdint.h>
#include "led.hpp"

//failure includes
#include "inc/stm32f10x_gpio.h"
#include "inc/stm32f10x_rcc.h"

int main()
{
    //enum USART_MODE test;
    //toll();
    Assert_Configuration();

    baseNvicInit();

    led_init();

    USART1_Init(USART_USE_INTERRUPTS);
    //USART1_Init(test);
    USART2_Init(USART_USE_INTERRUPTS);

    printf_setSendFunction(USART1_SendData);

    printf("Robot Firmware is alive\n");

    //test assert failure 
    failure();
    //end
    volatile uint32_t delay;

    while(1)
    {
        delay = 5000000;
        while(delay--)
            ;
        printf("Alive\n");

        //turn on red led
        led_set_red(true);
	led_set_green(false);

        delay = 5000000;
        while(delay--)
            ;

        //turn off red led
        led_set_red(false);
	led_set_green(true);
    }

    USART1_DeInit();
    USART2_DeInit();
    
    return 0;
}

void toll(){
	printf("Gruppe X ist toll");
	USART1_SendData((const unsigned char*)"Gruppe X ist toll", sizeof "Gruppe X ist toll");
    
}


void failure(){
    GPIO_InitTypeDef GPIO_InitStructure;
    //get default GPIO config
    GPIO_StructInit(&GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //LED (PA12)
    //configure as Push Pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
