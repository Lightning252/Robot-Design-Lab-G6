#include <stdio.h>
#include <stdlib.h>
extern "C" {
#include "../../drivers/usart.h"
}

FILE *read;

FILE *write;

void USART2_Init(enum USART_MODE mode){
	read = fopen("read.txt", "r");
	write = fopen("write.txt", "w");
	if(read == NULL || write == NULL){
		printf("Fehler beim oeffnen der Datei.");
		return;
	}
}

signed int USART2_GetData (unsigned char *buffer, const unsigned int buffer_length){
	if(fgets((char*)buffer, buffer_length, (FILE*)read) == NULL){
		printf("Fehler beim Lesen der Datei!");
		return -1;
	}
	return sizeof(buffer)/sizeof(char*);

}

signed int USART2_SendData(const unsigned char *data, const unsigned int size){
	char* text = (char*)malloc(sizeof(char*) * size);
	signed int save = fprintf (write, (const char*)data);
	if(save < 0){
		printf("Fehler beim Schreiben der Datei!");
		return -1;
	}
	return save;
}
