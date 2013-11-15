#include "receive.h"

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

