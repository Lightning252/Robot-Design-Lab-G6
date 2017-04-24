#include "drivers/usart.h"
#include "drivers/printf.h"
#include "led.hpp"
#include <stdlib.h> 
#include <string.h>
#include "protocol.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    listen();
}

void listen(){
    unsigned char buffer[1024]; 
    int i = USART2_GetData(buffer, 1024); 
    unsigned char text[i]; 
    for(int y = 0; y < i; y++){ 
      text[y] = buffer[y]; 
    }
    parseBuffer(buffer, i);
}

void parseBuffer(unsigned char buffer[], unsigned int buffer_length){
   char parse[buffer_length];
   //strcpy(buffer,(const char *)buffer);
   for(int i = 0; i < buffer_length;i++){ 
      parse[i] = buffer[i];
      if(strstr(parse,"LED AN")){
	printf("LED AN \n");

	parse[i] = ' ';
	
      }
      if(strstr(parse,"LED AUS")){
        printf("LED AUS \n");
	parse[i] = ' ';
      }  
   }

}

