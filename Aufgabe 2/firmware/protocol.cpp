#include "protocol.hpp"
#include "drivers/usart.h"
#include "drivers/printf.h"
#include "crc.h"

struct ProtocolHeader headerInternal;
unsigned int receivedHeaderBytes = 0;

send_func_t protocol_sendFunction;
recv_func_t protocol_recvFunction;

void (*funcMapping[PROTOCOL_IDS_SIZE])(enum PROTOCOL_IDS id, unsigned char *data, unsigned short size);

void protocol_init(send_func_t sendFunction, recv_func_t receiveFunction)
{
    receivedHeaderBytes = 0;
    protocol_sendFunction = sendFunction;
    protocol_recvFunction = receiveFunction;
}

struct ProtocolHeader *protocol_receiveHeader()
{
	printf("protocol_receiveHeader start\n");
    unsigned char *buffer = (unsigned char *) &headerInternal;
//     printf("Rec header\n");
    while(1)
    {
	int ret = protocol_recvFunction(buffer + receivedHeaderBytes, sizeof(struct ProtocolHeader) - receivedHeaderBytes);
	if(ret < 0)
	{
	    printf("Error, receive buffer ran over");
	    //discard all data and start over
	    receivedHeaderBytes = 0;
	    continue;
	}
	receivedHeaderBytes += ret;
        
	//not more data there, try again later
	if(ret == 0)
		printf("no header protocol_recvFunction \n");
	    return 0;

	//search for the magic byte
	unsigned int i;
	unsigned char found_magic = 0; 
	for(i = 0; i < receivedHeaderBytes; i++)
	{
	    if(buffer[i] == MAGIC_BYTE)
	    {
		found_magic = 1;
		//discard all data before the magic byte
		unsigned int j;
		for(j = i; j < receivedHeaderBytes; j++)
		{
		    buffer[j-i] = buffer[j];
		}
		receivedHeaderBytes -= i;
		break;
	    }
	}

	//no start marker found, discard buffer
	if(!found_magic)
	{
	    receivedHeaderBytes = 0;
	    continue;
	}

	//wait until we received a full header
	if(receivedHeaderBytes < sizeof(struct ProtocolHeader))
	    continue;

	struct ProtocolHeader *header = &headerInternal;
	
	//check crc
	if(header->headerCRC != protocol_calculateCRC(buffer, sizeof(struct ProtocolHeader)- sizeof(unsigned short)))
	{
	    printf("Got wrong CRC");
	    receivedHeaderBytes = 0;
	    continue;
	}
	
	receivedHeaderBytes = 0;
	
	return &headerInternal;
    }
    printf("protocol_receiveHeader end\n");
    return 0;
};

signed int protocol_receiveData(unsigned char* data, uint16_t* dataSize, short unsigned int maxBufferSize, uint64_t *type)
{
	printf("protocol_receiveData start\n");
    struct ProtocolHeader *header;
    int received = 0;    
    while(1)
    {
	//receive header
	header = protocol_receiveHeader();
	
	//no header there
	if(!header)
		printf("no header protocol_receiveData\n");
	    return 0;
	
//         printf("Got Header \n");
//         
//         printf("Waiting for %hu bytes\n", header->payloadSize);
        
        if(header->payloadSize > maxBufferSize)
        {
            printf("Protocol: Error, received packet has a payload bigger than the receive buffer\n");
            return 0;
        }
            
	int recv_error = 0;
	//got a valid header, receive the rest of the packet
	while(received < header->payloadSize)
	{
	    int ret = protocol_recvFunction(data + received, header->payloadSize - received);
	    if(ret < 0)
	    {
		recv_error = 1;
		break;
	    } else
            {
		received += ret;
//                 if(ret > 0)
//                 {
//                     printf("got %i bytes, %i bytes still missing\n", ret, header->payloadSize - received);
//                 }
            }
	}
	if (recv_error) {
	    printf("Error, receive buffer ran over\n");
	    //discard all data and start over
	    received = 0;
	    continue;
	}

	//check payload crc
	if(header->payloadCRC != protocol_calculateCRC(data, header->payloadSize))
	{
	    printf("Wrong Payload CRC \n");
	    received = 0;
	    continue;
	}
	
        if(header->payloadSize > maxBufferSize)
        {
            printf("Error, given buffer is to small\n");
            return 0;
        }
        
//         printf("Got cpmlete packet \n");
        *type = header->packetId;
        *dataSize =  header->payloadSize;
        return 1;
   }
   return 0;
}

short unsigned int protocol_calculateCRC(const unsigned char* data, short unsigned int size)
{
    return crcSlow(data, size);
}


void protocol_sendData(const unsigned char* data, short unsigned int size, enum PROTOCOL_IDS type)
{
    struct ProtocolHeader header;
    
    //setup a correct header
    header.magicByte = MAGIC_BYTE;
	header.packetId = type;
    header.payloadSize = size;
    header.payloadCRC = protocol_calculateCRC(data, size);
    header.headerCRC = protocol_calculateCRC((unsigned char *) &header, sizeof(struct ProtocolHeader)- sizeof(unsigned short));
    
    //send header
    unsigned int sent = 0;
    while(sent < sizeof(struct ProtocolHeader))
    {
	int ret = protocol_sendFunction(((unsigned char *) &header) + sent, sizeof(struct ProtocolHeader) - sent);
	printf("protocol_sendFunction first")
	if(ret < 0)
	{
	    printf("Error, sending failed");
	    sent = 0;
	}
	else
	    sent += ret;
    }
    
    //send payload
    sent = 0;
    while(sent < size)
    {
	int ret = protocol_sendFunction(data + sent, size - sent);
	printf("protocol_sendFunction second");
	if(ret < 0)
	{
	    printf("Error, sending failed");
	    sent = 0;
	}
	else
	    sent += ret;
    }
    printf("Data was sent.");
}

void protocol_registerFunc(unsigned int protocolId, funcSignature){
	if(PROTOCOL_IDS_SIZE > protocolId){
		funcMapping[protocolId] = handler;
	}
	else{
		printf("Error, not able to register function");
	}
}

void protocol_processData(){
	const int bufferSize = 1024;
    unsigned char buffer[bufferSize];
    uint16_t received = 0;
    uint64_t id = 0;
    int gotPacket = 0;
	while(1)
    {
	if((gotPacket = protocol_receiveData(buffer, &received, bufferSize, &id)) > 0){
		printf("package received");
		funcMapping[id](static_cast<PROTOCOL_IDS>(id), buffer, bufferSize);
	}
	}
}



