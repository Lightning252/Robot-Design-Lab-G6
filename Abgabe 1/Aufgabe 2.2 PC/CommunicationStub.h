#ifndef PCCOMMUNICATION_H
#define PCCOMMUNICATION_H
#include <stdio.h>

class Communication
{
public:
virtual int write(const unsigned char *data, size_t size) = 0;
virtual int read(unsigned char *buffer, size_t buffersize) = 0;
};

class PCCommunication : Communication{
public:
PCCommunication();
virtual int write(const unsigned char *data, size_t size);
virtual int read(unsigned char *buffer, size_t buffersize);
};

#endif
