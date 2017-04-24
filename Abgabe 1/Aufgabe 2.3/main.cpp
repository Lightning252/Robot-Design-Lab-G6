#include "USARTCommunication.h"

int main()
{
	USARTCommunication com;
    unsigned char ta[] = "echo \"hallo\"";
    unsigned char* t = ta;
    com.write(t, sizeof(ta));
}
