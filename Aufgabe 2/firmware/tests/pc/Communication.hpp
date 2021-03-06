#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP
#include <cstring>

class Communication
{
public:
    virtual ~Communication() {};
    virtual int write(const unsigned char *data, size_t size) = 0;
    virtual int read(unsigned char *buffer, size_t buffersize) = 0;
};

#endif