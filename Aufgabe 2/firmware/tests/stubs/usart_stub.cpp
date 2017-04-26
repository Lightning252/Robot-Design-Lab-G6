#include <iostream>
#include <assert.h>
#include <signal.h>
#include "IPCCom.hpp"

extern "C" {
#include "../../drivers/usart.h"
}

const std::string toRobotU1Name("to_robot_queueU1");
const std::string fromRobotU1Name("from_robot_queueU1");
const std::string toRobotU2Name("to_robot_queueU2");
const std::string fromRobotU2Name("from_robot_queueU2");

IPCSender *usart1;
IPCSender *usart2;

void USART1_Init(enum USART_MODE mode)
{
    usart1 = new IPCSender(fromRobotU2Name, toRobotU1Name);
}

void USART1_DeInit(void)
{
    delete usart1;
    usart1 = NULL;
};

signed int USART1_SendData(const unsigned char *data, const unsigned int size)
{
    return usart1->sendData(data, size);
}

signed int USART1_GetData (unsigned char *buffer, const unsigned int buffer_length)
{
    return usart1->getData(buffer, buffer_length);
}

void USART2_Init(enum USART_MODE mode)
{
    usart2 = new IPCSender(fromRobotU2Name, toRobotU2Name);
}

void USART2_DeInit(void)
{
    delete usart2;
    usart2 = NULL;
};

signed int USART2_SendData(const unsigned char *data, const unsigned int size)
{
    return usart2->sendData(data, size);
}

signed int USART2_GetData (unsigned char *buffer, const unsigned int buffer_length)
{
    return usart2->getData(buffer, buffer_length);
}
