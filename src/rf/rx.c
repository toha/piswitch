#include "rx.h"
#include "protocol.h"


/*typedef struct
{
    unsigned long rawdata;

} RecvProtocol;



// start      network          payload     CRC
//   1      101100000000        110100      1

// payload:
// lamp address    Broadcast     On/Off    Dimmer     Immer 0
//      11             1            1         1          0
typedef struct
{
    RecvProtocol base;

    // derived members
	unsigned long network;
	unsigned int address;
	unsigned int broadcast;
	unsigned int state;
	unsigned int dimmer;

} RecvProtocol5;*/





void recvCallback(unsigned int protocol, unsigned long data)
{
	printf("protocol: %d, value: %lu\n", protocol, data);
	printBits(sizeof(data), &data);

	protocolDecoder(protocol, data);
}

void protocolDecoder(unsigned int protocol, unsigned long data) 
{
	if (protocol == 1) {

	} else if (protocol == 2) {

	} else if (protocol == 3) {

	} else if (protocol == 4) {

	} else if (protocol == 5) {

	}
}

int startRfReceive()
{
	wiringPiSetup();
	RCSwitch mySwitch = RCSwitch();
	mySwitch.enableReceive(0);
	mySwitch.fnReceiveCallback = &recvCallback;

	while (1) {
		sleep(1);
	}

    return 0;
}

