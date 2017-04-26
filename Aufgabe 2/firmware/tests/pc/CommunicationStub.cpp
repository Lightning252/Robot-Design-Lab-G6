#include "CommunicationStub.hpp"

CommunicationStub::CommunicationStub(IPCSender *ipcSender):ipcSender(ipcSender)
{
};

CommunicationStub::~CommunicationStub()
{
    delete ipcSender;
}


int CommunicationStub::read(unsigned char* buffer, size_t buffersize)
{
    return ipcSender->getData(buffer, buffersize);
}

int CommunicationStub::write(const unsigned char* data, size_t size)
{
    return ipcSender->sendData(data, size);
}

