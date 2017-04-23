#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class Communication
{
public:
    virtual int cwrite(unsigned char *data, size_t size) = 0;
    virtual int cread(unsigned char *buffer, size_t buffersize) = 0;
};

#endif