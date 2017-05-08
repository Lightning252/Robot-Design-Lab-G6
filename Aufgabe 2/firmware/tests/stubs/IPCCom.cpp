#include "IPCCom.hpp"
#include <iostream>
#include <signal.h>
#include <vector>

std::vector<std::string> registeredQueues;

using namespace boost::interprocess;

void exit_handler(int sig)
{
    for(std::vector<std::string>::iterator it = registeredQueues.begin() ; it != registeredQueues.end(); it++)
    {
        if(!message_queue::remove(it->c_str()))
            std::cout << "Warning, could not remove " << *it << std::endl;
    }
    
    exit(sig);
}

IPCSender::IPCSender(const std::string senderQueueName, const std::string receiverQueueName): senderQueueName(senderQueueName), receiverQueueName(receiverQueueName)
{
    signal(SIGINT, exit_handler);
    receiverQueue = new message_queue(
        open_or_create            //only create
        ,receiverQueueName.c_str()          //name
        ,1024                       //max message number
        ,sizeof(char)              //max message size
        );      
    senderQueue = NULL;
    int cnt = 0;
    std::cout << "Waiting for ipc connection" << std::endl;
    while(!senderQueue)
    {
        try {
            senderQueue = new message_queue(open_only ,senderQueueName.c_str());
        } catch (interprocess_exception &ex) {
            usleep(10000);
            cnt++;
            if(cnt > 100)
            {
                cnt = 0;
                std::cout << "Waiting for ipc connection" << std::endl;
            }
        }
    }
    
    registeredQueues.push_back(senderQueueName);
    registeredQueues.push_back(senderQueueName);
    
    std::cout << "Connected" << std::endl;
};
    
IPCSender::~IPCSender()
{
    delete senderQueue;
    delete receiverQueue;
}

signed int IPCSender::sendData(const unsigned char *data, const unsigned int size)
{
    unsigned int nrToSend = rand() % (size + 1);
    assert(nrToSend <= size);
    
    try {
        for(unsigned int i = 0; i < nrToSend; i++)
        {
            if(!senderQueue->try_send(data + i, sizeof(char), 0))
            {
                std::cout << "Warning IPC Buffer full" << std::endl;
                return i;
            }
        }
    } catch (interprocess_exception &ex) {
        std::cout << "send" << ex.what() << std::endl;
        return -1;
    }
    
//     for(unsigned int i = 0; i < nrToSend; i++)
//         std::cout << data[i] << std::flush;
    
    return nrToSend;
}

signed int IPCSender::getData (unsigned char *buffer, const unsigned int buffer_length)
{
    unsigned int nrReveived = rand() % (buffer_length + 1);
    assert(nrReveived <= buffer_length);
    
    unsigned int priority;
    size_t received = 0;
    try {
        if(!receiverQueue->try_receive(buffer, sizeof(char), received, priority))
        {
            //std::cout << "getData 0" << std::endl;
            return 0;
        }
    } catch (interprocess_exception &ex) {
        std::cout << "recv " << ex.what() << std::endl;
        return -1;
    }
    std::cout << "getData received" << std::endl;
    return received;
}

