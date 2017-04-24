#include "crc.h"
#include "USARTCommunication.h"
#include "protocol.hpp"
#include "string.h"

/*
    8 BYTES PAYLOAD
    4 BYTES CHECKSUM
*/
void send(unsigned char const message[], int nBytes){
    //send magic word
    USARTCommunication com;
    unsigned char magicPackage[PACKAGESIZE];
    unsigned char magicPayload[PAYLOADSIZE] = MAGICWORD;
    fillPackage(magicPackage, magicPayload);    
    com.write(magicPackage, sizeof(magicPackage));
    
    //first loop to divide the message in sub packages
    for(int i = 0; i < nBytes; i += PACKAGESIZE){

        unsigned char package[PACKAGESIZE];
        unsigned char payload[PAYLOADSIZE];
        //second loop to actually send a package
        for(int y = 0; y < PAYLOADSIZE; y++){
            payload[y] = message[i + y];
        }
        fillPackage(package, payload);
        com.write(package, sizeof(package));
    }
}

bool magicwordReceived = false;

void receive(unsigned char text[]){
    unsigned char package[PACKAGESIZE];     
    unsigned char payload[PAYLOADSIZE];
    USARTCommunication com;
    com.read(package, PACKAGESIZE);
    extractPayload(package, payload);
    unsigned char checksum[CHECKSUMSIZE];
    extractChecksum(package, checksum);
    if(strcmp((const char *)payload, MAGICWORD) == 0){
        magicwordReceived = true;
    }

    if(checkChecksum(payload, checksum) && magicwordReceived){
        text = payload;
    }
}


bool checkChecksum(unsigned char payload[], unsigned char checksum[]){
    crc c = crcSlow(payload, sizeof((unsigned char *)payload));
    unsigned long int l = checksum[0] | (checksum[1] << 8) | (checksum[2] << 16) | (checksum[3] << 24);
    return c == l;
}

void extractPayload(unsigned char package[], unsigned char payload[]){
    for(int i = 0; i < PAYLOADSIZE; i++){
        payload[i] = package[i];
    }
}

void extractChecksum(unsigned char package[], unsigned char checksum[]){
     for(int i = PAYLOADSIZE; i < CHECKSUMSIZE; i++){
        checksum[i] = package[i];
    }
}

void fillPackage(unsigned char package[], unsigned char payload[]){
    crc c = crcSlow(payload, sizeof((unsigned char *)payload));
    unsigned char * checksum = (unsigned char*)&checksum;

    for(int i = 0; i < PAYLOADSIZE; i++){
        package[i] = payload[i];
    }
    for(int i = PAYLOADSIZE; i < PACKAGESIZE; i++){
        package[i] = checksum[i - PACKAGESIZE];
    }
}
