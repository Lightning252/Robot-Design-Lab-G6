#include "drivers/printf.h"
#include "drivers/usart.h"
#include <stdlib.h>
#include "stm32_specific.h"
#include <stdint.h>
#include "led.hpp"
#include "protocol.hpp"

Step* moveStack[10];

void stepFunction(enum PROTOCOL_IDS id, unsigned char *data, unsigned short size){
    printf("this is the stepFunction");
    moveStack[0] = (Step*)data;
}

void testFunction(enum PROTOCOL_IDS id, unsigned char *data, unsigned short size){
    printf("this is the testFunction");
}

void failFunction(enum PROTOCOL_IDS id, unsigned char *data, unsigned short size){
    printf("this should never be printed");
}
int main()
{
    Assert_Configuration();

    baseNvicInit();

    led_init();

    USART1_Init(USART_USE_INTERRUPTS);
    USART2_Init(USART_USE_INTERRUPTS);

    printf_setSendFunction(USART1_SendData);

    printf("Robot Firmware is alive\n");

    protocol_init(USART2_SendData, USART2_GetData);

    protocol_registerFunc(ID_TEST, testFunction);
    protocol_registerFunc(ID_STEP, stepFunction); 
    //should fail
    protocol_registerFunc(PROTOCOL_IDS_SIZE, failFunction); 
    while(1)
    {
        protocol_processData();
    }

    USART1_DeInit();
    USART2_DeInit();

    return 0;
}


