#include "USARTCommunication.h"
#include <unistd.h>

int main()
{
	USARTCommunication com;
    unsigned char on[] = "LED AN";
    unsigned char* onData = on;
    com.write(onData, sizeof(on));


    usleep(5000);
    
    unsigned char off[] = "LED AUS";
    unsigned char* offData = off;
    com.write(offData, sizeof(off));

}
