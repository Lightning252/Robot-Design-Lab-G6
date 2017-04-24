#ifndef PCCOMMUNICATION_H
#define PCCOMMUNICATION_H
#include <stdio.h>

class Communication
{
public:
/*
Schreibt size lang aus data in eine Datei.
*/
virtual int write(const unsigned char *data, size_t size) = 0;

/*
Liest buffersize lang aus einer Datei und schreibt den Inhalt in den buffer
*/
virtual int read(unsigned char *buffer, size_t buffersize) = 0;
};

class PCCommunication : Communication{
public:
PCCommunication();
/*
Schreibt size lang aus data in eine Datei.
*/
virtual int write(const unsigned char *data, size_t size);

/*
Liest buffersize lang aus einer Datei und schreibt den Inhalt in den buffer
*/
virtual int read(unsigned char *buffer, size_t buffersize);
};

#endif
