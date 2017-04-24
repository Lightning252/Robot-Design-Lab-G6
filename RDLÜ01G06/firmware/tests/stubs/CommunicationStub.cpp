

#include "CommunicationStub.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
using namespace std;

/*
int main(int argc, char **argv)
{
unsigned char *buffer;
buffer = (unsigned char*) malloc (sizeof (char) * 7);
    if (buffer == NULL) {
  	fputs ("Memory error",stderr); exit (2);
    }

    PCCommunication *pcc =  new PCCommunication();

    //if(pcc->write(reinterpret_cast<const unsigned char *>("LED AN"),7) <= 0){
	//fputs ("write error",stderr); exit (2);
    //}

    if(pcc -> read(buffer, 7) <= 0){
        fputs ("read error",stderr); exit (2);
    } 
    printf(reinterpret_cast<const char *>(buffer));
    free(buffer);
    return 0;
}
*/
PCCommunication::PCCommunication(){
}

int PCCommunication :: write(const unsigned char *data, size_t size){
  FILE * write;	
  write = fopen ("write.txt", "w");
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


