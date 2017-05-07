#include "ComProtocol.hpp"
#include "UARTCommunication.hpp"
#include "CommunicationStub.hpp"
#include <string.h>
#include <iostream>

void initStep(struct Step &step)
{
    
    memset(&step, 0, sizeof(Step));
    step.length = 500;
    for(int i = 0; i < NUM_SERVOS; i++)
    {
	step.servoTrajectory[i].size = 13;

	for(int j = 0; j < 12; j++)
	{
	    step.servoTrajectory[i].data[j].time = 10 + j * 20;
	    step.servoTrajectory[i].data[j].angle = 15 + j * 150;
	}
	step.servoTrajectory[i].data[12].time = 10 + 20 * 20;
	step.servoTrajectory[i].data[12].angle = 15;
    }
    
}

int main(int argc, char** argv)
{
    Step* step = new Step();
    memset(step, 0, sizeof(Step));

    step->length = 500;
    for(int i = 0; i < NUM_SERVOS; i++)
    {
	step->servoTrajectory[i].size = 13;

	for(int j = 0; j < 12; j++)
	{
	    step->servoTrajectory[i].data[j].time = 10 + j * 20;
	    step->servoTrajectory[i].data[j].angle = 15 + j * 150;
	}
	step->servoTrajectory[i].data[12].time = 10 + 20 * 20;
	step->servoTrajectory[i].data[12].angle = 15;
    }

    Communication* com = new UARTCommunication("/dev/ttyUSB1");
    ComProtocol *proto = ComProtocol::getInstance(com);
    
    //test ID_STEP
    const int bufSize = 200;
    std::vector<uint8_t> data;
    data.resize(sizeof(Step));
    memcpy(data.data(), step, data.size());
    std::cout << "Sending test data" <<std::endl;
    proto->sendData(data, ID_STEP);

    //test ID_TEST
    const int bufSizeTest = 200;
    std::vector<uint8_t> dataTest;
    data.resize(bufSizeTest);
    
    for(int i = 0; i < bufSizeTest; i++)
    {
        dataTest[i] = i;
    }
    std::cout << "Sending test data" <<std::endl;
    proto->sendData(dataTest, ID_TEST);
}

void readPrintfs(Communication* com)
{
    unsigned char buf;
    while(com->read(&buf, sizeof(char)))
	std::cout << buf << std::flush;
}
/*
int main(int argc, char** argv)
{
    if(argc < 1 || argc > 3)
    {
        std::cerr << "call testpc [/dev/ttyUSBx]" << std::endl;
        return 0;
    }

    Communication* com;
    if(argc == 1)
    {
        IPCSender *sender = new IPCSender("to_robot_queueU2", "from_robot_queueU2");
        com = new CommunicationStub(sender);
    } else
    {
        com = new UARTCommunication(argv[1]);
    }

    ComProtocol *proto = ComProtocol::getInstance(com);

    readPrintfs(com);
    
    const int bufSize = 200;
    std::vector<uint8_t> data;
    data.resize(bufSize);
    
    for(int i = 0; i < bufSize; i++)
    {
        data[i] = i;
    }
    

    std::cout << "Sending test data" <<std::endl;
    proto->sendData(data);
    
    std::vector<uint8_t> recvData;
    
//     usleep(10000);
//     com->write(((const unsigned char *)"test"), 4);
    
    //read robot debug messages
    while(1)
    {
        if(proto->getData(recvData))
        {
            std::cout << "Got test data back" << std::endl;
            break;
        }
        else
            usleep(10000);
    }

    if(recvData.size() != data.size())
    {
        std::cout << "Error data size does not match" << std::endl;
        return 0;
    }
    
    std::cout << "Verifying test data :" << std::endl;
    bool error = false;
    for(int i = 0; i < bufSize; i++)
    {
        if(data[i] != recvData[i])
        {
            std::cout << "Error data at position " << i << " does not match" << std::endl;
            int a = data[i];
            int b = recvData[i];
            std::cout << "A is " << a << " b is " << b << std::endl;
            error = true;
        }
    }

    if(!error)
        std::cout << "All ok" << std::endl;
    delete proto;
    
    return 0;
}
*/
