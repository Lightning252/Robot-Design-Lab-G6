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
buffer[0] = 'E'; 
buffer[1] = 'I'; 
buffer[2] = 'N'; 
buffer[3] = ' '; 
buffer[4] = 'A'; 
buffer[5] = 'U'; 
buffer[6] = 'S'; 
buffer[7] = ' '; 	
buffer[8] = 'L';
buffer[9] = 'E';
buffer[10] = 'D';
buffer[11] = 'L';
buffer[12] = 'E';
buffer[13] = 'D';
buffer[14] = ' ';
buffer[15] = 'A';
buffer[16] = 'U';
buffer[17] = 'S';
//buffer[18] = '';
//alex@alex-VirtualBox:~/Documents/DesignLabGit/Robot-Design-Lab-G6/Z1/firmware$ g++ protocol.cpp protocol.hpp
//alex@alex-VirtualBox:~/Documents/DesignLabGit/Robot-Design-Lab-G6/Z1/firmware$ ./a.out
//LED AUSalex@alex-VirtualBox:~/Documents/DesignLabGit/Robot-Design-Lab-G6/Z1/firmware$ ^C


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

