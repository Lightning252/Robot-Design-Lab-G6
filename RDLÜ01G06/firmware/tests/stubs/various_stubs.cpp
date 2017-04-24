#include <iostream>
#include <stdlib.h>
#include "../../drivers/printf.h"

extern "C"
{
void Assert_Configuration()
{
}


void baseNvicInit()
{
}

void GPIO_Configuration(void)
{
}
    

void assert_failed(unsigned char* file, unsigned int line)
{
    std::cout << "Assertion failed in file " << file << " on line " << line << std::endl;
    exit(1);
}

void printf_setSendFunction(printf_send_func_t function)
{
}

}


