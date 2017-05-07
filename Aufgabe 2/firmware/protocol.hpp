#ifndef _PROTOCOL_HPP__
#define _PROTOCOL_HPP__

#include <stdint.h>
#include "engine.hpp"

#define MAX_PACKET_SIZE (1024 + sizeof(struct ProtocolHeader))
#define MAGIC_BYTE 0x3f

#define funcSignature void (*handler)(enum PROTOCOL_IDS id, unsigned char *data, unsigned short size)
#define BUFFERSIZE 1024

/**
 * Package types
 **/
enum PROTOCOL_IDS{
    ID_TEST,
    ID_STEP,
    PROTOCOL_IDS_SIZE
};

/**
 * This is a structure that defines the header of 
 * our protocol. 
 * This header is allways send prior to the 
 * 'payload' of our protocol.
 * */
struct ProtocolHeader
{
    ///A magic byte used to find the beginning of an new packet
    unsigned char magicByte;
    ///Set the package type
    enum PROTOCOL_IDS packetId : 8;
    ///The size in bytes of the payload
    unsigned short payloadSize;
    ///A checksum of the payload. This is used to find 
    ///transmission errors
    unsigned short payloadCRC;
    /**
     * The checksum of the header itself. This is used for two purposed:
     * a: detecting transmission errors
     * b: detection of wrong magic bytes
     *    If we misinterpret some byte in the stream a a packet
     *    start (magicByte), the checksum will not match the header.
     *    Thus wrong detected magic bytes can be eleminated. 
     * */
    unsigned short headerCRC;
} __attribute__ ((packed)) __attribute__((__may_alias__));

typedef signed int (*send_func_t)(const unsigned char *data, const unsigned int size);
typedef signed int (*recv_func_t)(unsigned char *data, const unsigned int dataSize);

/**
 * Initializes internal data structures.
 * Must be called prior to any protocol_
 * function.
 * */
void protocol_init(send_func_t sendFunction, recv_func_t receiveFunction);

/**
 * this function will check weather a packet can be read 
 * from usart 1. If this is the case the function will
 * block until the whole packet is received and return the packet.
 * Will return 0 if no packet can be received.
 */
signed int protocol_receiveData(unsigned char* data, uint16_t *dataSize, short unsigned int maxBufferSize, uint64_t *type);

/**
 * Calculates a CRC over the given array of bytes
 * @arg data A pointer to the data for which the crc should be calculated
 * @arg size Size of the data
 * @returns The CRC
 * */
unsigned short protocol_calculateCRC(const unsigned char *data, unsigned short size);

/**
 * Sends the given data using the given id.
 * @arg data A pointer to the data to be send
 * @arg size Size of the data to be send 
 * @arg type of package to send
 * */
void protocol_sendData(const unsigned char *data, unsigned short size, enum PROTOCOL_IDS type);

/**
 * Maps a function for specific id
 * @arg the id where the function will be mapped
 * @arg the function that will mapped
 **/
void protocol_registerFunc(unsigned int protocolId, funcSignature);

/**
 * Receives a packages and executes the mapped function for the id
 **/
void protocol_processData();

//void (*handler)(enum PROTOCOL_IDS id, unsigned char *data, unsigned short size) functions[3];
#endif
