#include "USARTCommunication.h"

int main(int argc, char **argv)
{
    USARTCommunication com = USARTCommunication();
    com.start();
    return 0;
}



