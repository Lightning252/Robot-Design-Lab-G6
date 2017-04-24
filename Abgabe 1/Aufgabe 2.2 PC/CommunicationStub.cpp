

#include "CommunicationStub.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
using namespace std;

int main(int argc, char **argv)
{
unsigned char *buffer;
buffer = (unsigned char*) malloc (sizeof(char)*7);
    if (buffer == NULL) {
  	fputs ("Memory error",stderr); exit (2);
    }

    PCCommunication *pcc =  new PCCommunication();

    if(pcc -> read(buffer, 7) <= 0){
	fputs ("read error",stderr); exit (2);
    } 

    if(pcc->write(reinterpret_cast<const unsigned char *>("testerererererer"),6) <= 0){
	fputs ("write error",stderr); exit (2);
    }

    free(buffer);
    return 0;
}

PCCommunication::PCCommunication(){
}

int PCCommunication :: write(const unsigned char *data, size_t size){
  FILE * write;	
  write = fopen ("read.txt", "w");
  int hasWritten = fwrite (data , sizeof(char), sizeof(data), write);
  fclose (write);
  return hasWritten;
}


int PCCommunication :: read(unsigned char *buffer, size_t buffersize){
  FILE * read;	
  read = fopen ("write.txt", "r");
  int hasRead= fread (buffer , sizeof(char), sizeof(buffer), read);
  
  fclose (read);
  return hasRead;
}

void toll(){
	printf("Gruppe X ist toll");
	USART1_SendData((const unsigned char*)"Gruppe X ist toll", sizeof "Gruppe X ist toll");
    
}

