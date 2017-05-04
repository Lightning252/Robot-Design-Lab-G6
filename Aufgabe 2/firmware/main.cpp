#include "drivers/printf.h"
#include "drivers/usart.h"
#include <stdlib.h>
#include "stm32_specific.h"
#include <stdint.h>
#include "led.hpp"
#include "protocol.hpp"

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
    while(1)
    {
      if((gotPacket = protocol_receiveData(buffer, &received, bufferSize, &id)) > 0)
        {
            printf("Got Data, size %i\n", received);
            printf("Sending it back\n");
            protocol_sendData(buffer, received);
        }

    }

    USART1_DeInit();
    USART2_DeInit();

    return 0;
}


