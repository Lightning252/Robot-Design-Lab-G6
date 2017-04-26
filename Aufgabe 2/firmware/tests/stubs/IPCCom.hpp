#ifndef IPC_COM_HPP
#define IPC_COM_HPP

#include <boost/interprocess/ipc/message_queue.hpp>
#include <string>

class IPCSender {
public:
    IPCSender(const std::string senderQueueName, const std::string receiverQueueName);
    
    ~IPCSender();
    
    signed int sendData(const unsigned char *data, const unsigned int size);
    signed int getData (unsigned char *buffer, const unsigned int buffer_length);
    
private:
    const std::string senderQueueName;
    const std::string receiverQueueName;
    boost::interprocess::message_queue *senderQueue;
    boost::interprocess::message_queue *receiverQueue;
};

#endif