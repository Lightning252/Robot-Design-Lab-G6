#include "USARTCommunication.h"

int main()
{
	USARTCommunication com;
    unsigned char ta[] = "test";
    unsigned char* t = ta;
    com.write(t, 5);
}