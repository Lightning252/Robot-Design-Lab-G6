#include "drivers/printf.h"
#include "drivers/usart.h"
#include <stdlib.h>
#include "stm32_specific.h"
#include <stdint.h>
#include "led.hpp"

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


    volatile uint32_t delay;

    /*
        Begin - Aufgabe 2.3
    */
 //   USARTCommunication_start();
    /*
        End - Aufgabe 2.3
    */
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


