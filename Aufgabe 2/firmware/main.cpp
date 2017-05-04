#include "drivers/printf.h"
#include "drivers/usart.h"
#include <stdlib.h>
#include "stm32_specific.h"
#include <stdint.h>
#include "led.hpp"
#include "protocol.hpp"

void testFunction(enum PROTOCOL_IDS id, unsigned char *data, unsigned short size){
    Step* step = (Step*)data;
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

    const int bufferSize = 1024;
    unsigned char buffer[bufferSize];

    uint16_t received = 0;
    uint64_t id = 0;
    int gotPacket = 0;

    protocol_registerFunc(ID_STEP, testFunction); 
    while(1)
    {
        protocol_processData();
    }

    USART1_DeInit();
    USART2_DeInit();

    return 0;
}


