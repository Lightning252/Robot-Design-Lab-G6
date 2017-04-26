#include "led.hpp"
#include "inc/stm32f10x_gpio.h"
#include "inc/stm32f10x_rcc.h"

void led_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //get default GPIO config
    GPIO_StructInit(&GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    //LED (PA12)
    //configure as Push Pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void led_set_red(bool on)
{
    if(on)
    {
        //turn on red led
        GPIO_ResetBits(GPIOA, GPIO_Pin_12);
    }
    else
    {
        //off
        GPIO_SetBits(GPIOA, GPIO_Pin_12);
    }
}
