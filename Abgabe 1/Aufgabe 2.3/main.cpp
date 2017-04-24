#include "USARTCommunication.h"

int main()
{
	USARTCommunication com;
    unsigned char ta[] = "LED AN";
    unsigned char* t = ta;
    com.write(t, sizeof(ta));
}
