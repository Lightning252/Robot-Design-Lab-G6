#ifndef USARTCOMMUNICATION_H
#define USARTCOMMUNICATION_H

#include <stdio.h>
#include "Communication.h"

class USARTCommunication : Communication{
    public:
        USARTCommunication();
	/*
	Schreibt size lang den Inhalt aus data 
	*/
        virtual int write(unsigned char *data, size_t size);

	/*
	Liest buffersize lang und schreibt dies in den übergebenen buffer
	*/
        virtual int read(unsigned char *buffer, size_t buffersize);
    private:
        int tty_fd;
};

#endif
