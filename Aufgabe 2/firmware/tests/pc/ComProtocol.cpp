#include "ComProtocol.hpp"
#include <iostream>
#include <boost/bind.hpp>

ComProtocol* ComProtocol::singletonVar;
ComProtocol *ComProtocol::getInstance(Communication* com)
{
    if(!singletonVar)
        singletonVar = new ComProtocol(com);
    
    return singletonVar;
}


ComProtocol::ComProtocol(Communication* com)
{
    this->com = com;
    
    protocol_init(write, read);
}

ComProtocol::~ComProtocol()
{
//     std::cout << "Removing Com Protocoll" << std::endl;
    delete com;
}

int ComProtocol::read(unsigned char* buffer, unsigned int buffersize)
{
    return singletonVar->com->read(buffer, buffersize);
}

int ComProtocol::write(const unsigned char* data, unsigned int size)
{
    return singletonVar->com->write(data, size);
}
 
void ComProtocol::sendData(const std::vector< uint8_t > data, enum PROTOCOL_IDS type)
{
    protocol_sendData(data.data(), data.size(), type);
}

bool ComProtocol::getData(std::vector< uint8_t >& data, uint64_t *type)
{
    const size_t maxBufferSize = MAX_PACKET_SIZE;
    if(data.size() < maxBufferSize)
        data.resize(maxBufferSize);
    uint16_t receivedBytes;
    int gotPacket = protocol_receiveData(data.data(), &receivedBytes, data.size(), type);
    if(gotPacket)
    {
        data.resize(receivedBytes);
        return true;
    }    
    return false;
}
