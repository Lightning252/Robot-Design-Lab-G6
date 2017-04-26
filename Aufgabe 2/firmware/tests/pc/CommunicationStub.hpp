#ifndef COMMUNICATIONSTUB_HPP
#define COMMUNICATIONSTUB_HPP

#include "Communication.hpp"
#include <boost/interprocess/ipc/message_queue.hpp>
#include "../stubs/IPCCom.hpp"

class CommunicationStub : public Communication
{
private:
    IPCSender *ipcSender;
public:
    CommunicationStub(IPCSender *ipcSender);
    virtual ~CommunicationStub();
    virtual int write(const unsigned char *data, size_t size);
    virtual int read(unsigned char *buffer, size_t buffersize);
};

#endif // COMMUNICATIONSTUB_HPP
