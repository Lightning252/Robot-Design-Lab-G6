#ifndef COMPROTOCOL_HPP
#define COMPROTOCOL_HPP

#include "Communication.hpp"
#include <vector>
#include <stdint.h>
#include "../../protocol.hpp"

class ComProtocol
{
private:
    static ComProtocol *singletonVar;
    class Communication *com;
    static int write(const unsigned char *data, unsigned int size);
    static int read(unsigned char *buffer, unsigned int buffersize);
    ComProtocol(class Communication *com);
public:
    static ComProtocol *getInstance(class Communication *com);
    ~ComProtocol();
    void sendData(const std::vector<uint8_t> data, enum PROTOCOL_IDS type);
    bool getData(std::vector<uint8_t> &data, uint64_t *type);
};

#endif // COMPROTOCOL_HPP
