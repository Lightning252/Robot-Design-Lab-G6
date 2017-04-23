#include "USARTCommunication.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

USARTCommunication::USARTCommunication(){
    this->tty_fd=open("/dev/ttyUSB0", O_RDWR | O_NONBLOCK);
    if(tty_fd < 0)
        throw std::runtime_error("Error opening tty");

    struct termios tio;

    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    cfsetospeed(&tio,B921600);
    cfsetispeed(&tio,B921600);
    tcsetattr(tty_fd,TCSANOW,&tio);
}

int USARTCommunication::cwrite(unsigned char *data, size_t size){
    return write(tty_fd, data, size);
}
int USARTCommunication::cread(unsigned char *buffer, size_t buffersize){
    return read(tty_fd, buffer, buffersize);
}


