#include "UARTCommunication.hpp"

#include <fstream>
#include <fcntl.h>

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdexcept>


UARTCommunication::UARTCommunication(const std::string &device)
{
    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    tty_fd=open(device.c_str(), O_RDWR | O_NONBLOCK | O_SYNC);
    cfsetospeed(&tio,B921600);            // 115200 baud
    cfsetispeed(&tio,B921600);            // 115200 baud

    tcsetattr(tty_fd,TCSANOW,&tio);

    if(tty_fd < 0)
        throw std::runtime_error("Error opening tty " + device);
};

UARTCommunication::~UARTCommunication() {
    if(close(tty_fd))
        std::cout << "Warning, could not close tty" << std::endl;
    std::cout << "Closed tty" << std::endl;
}


int UARTCommunication::read(unsigned char* buffer, size_t buffersize)
{
    int ret = 0;
    ssize_t recvd;

    if(0 >= (recvd = ::read(tty_fd,buffer + ret, sizeof(char))))
    {
        return 0;
    }

    ret += recvd;

    return ret;
}

int UARTCommunication::write(const unsigned char* data, size_t size)
{
    int ret = ::write(tty_fd, data, size);
    
    
    if(tcdrain(tty_fd) < 0)
    {
        perror("TCDrain failed");
    }
    
    return ret;
}