
#define CHECKSUMSIZE 4
#define PAYLOADSIZE 8
#define PACKAGESIZE PAYLOADSIZE + CHECKSUMSIZE
#define MAGICWORD "KAPPAPA"

void send(unsigned char const message[], int nBytes);
void receive(unsigned char text[]);
void extractPayload(unsigned char package[], unsigned char payload[]);
void extractChecksum(unsigned char package[], unsigned char checksum[]);
void fillPackage(unsigned char package[], unsigned char payload[]);
bool checkChecksum(unsigned char payload[], unsigned char checksum[]);