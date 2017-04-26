#ifndef UARTCOMMUNICATION_HPP
#define UARTCOMMUNICATION_HPP

#include "Communication.hpp"
#include <termios.h>
#include <string>

class UARTCommunication : public Communication
{
private:
    int tty_fd;
    struct termios tio;
public:
    UARTCommunication(const std::string &device);
    virtual ~UARTCommunication();
    virtual int write(const unsigned char *data, size_t size);
    virtual int read(unsigned char *buffer, size_t buffersize);
};

#endif // UARTCOMMUNICATION_HPP
