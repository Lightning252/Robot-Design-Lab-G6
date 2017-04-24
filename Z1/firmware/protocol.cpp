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
    unsigned char buffer[5000];
    unsigned int completeSize =0;
   //int size = USART2_GetData(buffer, 50000);
buffer[0] = 'U'; 
buffer[1] = 'L'; 
buffer[2] = 'E'; 
buffer[3] = 'D'; 
buffer[4] = ' '; 
buffer[5] = 'L'; 
buffer[6] = 'E'; 
buffer[7] = 'D'; 	
buffer[8] = ' ';
buffer[9] = 'A';
buffer[10] = 'N';
buffer[11] = 'L';
buffer[12] = 'E';
buffer[13] = 'D';
buffer[14] = ' ';
buffer[15] = 'A';
buffer[16] = 'U';
buffer[17] = 'S';
buffer[18] = 'N';

int  size = 19;
parseBuffer(buffer, size);
}

void parseBuffer(unsigned char buffer[], unsigned int buffer_length){
   char parse[buffer_length];
   //strcpy(buffer,(const char *)buffer);
   for(int i = 0; i < buffer_length;i++){ 
      parse[i] = buffer[i];
      if(strstr(parse,"LED AN")){
	printf("LED AN");

	parse[i] = ' ';
	
      }
      if(strstr(parse,"LED AUS")){
        printf("LED AUS");
	parse[i] = ' ';
      }  
   }

}

