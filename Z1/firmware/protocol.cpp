#include "drivers/usart.h"
#include "drivers/printf.h"
#include "led.hpp"
#include <stdlib.h> 

void listen(){
    unsigned char buffer[20];
    
    for(int i = 0; i < 3; i++){
        printf(buffer[i]);
    }

    int size = USART2_GetData(buffer, 7);
    //print(func);
    if(buffer == (unsigned char*)"LED AN"){
        led_set_red(true);
    }
    else if(buffer == (unsigned char*)"LED AUS"){
        led_set_red(false);
    }
}

void parseBuffer(unsigned char buffer[]){
/*
    for(int i = 0; i < 3;i++){
        printf(buffer[i]);
    }
*/
}

