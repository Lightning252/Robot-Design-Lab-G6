#ifndef USARTCOMMUNICATION_H
#define USARTCOMMUNICATION_H

#include <stdio.h>
#include "Communication.h"

class USARTCommunication : Communication{
    public:
        USARTCommunication();
        virtual int write(unsigned char *data, size_t size);
        virtual int read(unsigned char *buffer, size_t buffersize);
    private:
        int tty_fd;
};

#endif
