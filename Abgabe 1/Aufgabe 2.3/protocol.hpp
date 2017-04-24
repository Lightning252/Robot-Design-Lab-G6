
#define CHECKSUMSIZE 4
#define PAYLOADSIZE 8
#define PACKAGESIZE PAYLOADSIZE + CHECKSUMSIZE
#define MAGICWORD "KAPPAPA"

/*
    Teilt die message in Pakete auf uns verschickt sie. Hängt außerdem die Checksummen an die Pakete
*/
void send(unsigned char const message[], int nBytes);
/*
    Empfängt Pakete. Überprüft ob bereits das Magicword erhalten wurde und die Checksumme stimmt.
*/
void receive(unsigned char text[]);
/*
    Extrahiert die Payload aus dem Paket
*/
void extractPayload(unsigned char package[], unsigned char payload[]);
/*
    Extrahiert die Checksumme aus dem Paket
*/
void extractChecksum(unsigned char package[], unsigned char checksum[]);
/*
    Füllt das Paket mit der Payload und der Checksumme
*/
void fillPackage(unsigned char package[], unsigned char payload[]);
/*
    Überprüft, ob die berechnete Checksumme mit der übergebeben übereinstimmt.
*/
bool checkChecksum(unsigned char payload[], unsigned char checksum[]);