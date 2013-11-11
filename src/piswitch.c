#include "piswitch.h"
#include <wiringPi.h>
#include "lib/rcswitch-pi/RCSwitch.h"
int main()
{
	printf("\nPiSwitch\n");
	//startweb();

	wiringPiSetup();
	RCSwitch mySwitch = RCSwitch();
	mySwitch.enableReceive(0);

	while(1) {

		if (mySwitch.available()) {
			//printf("joo\n");
			unsigned long value = mySwitch.getReceivedValue();
		
			if (value == 0) {
			  printf("Unknown encoding\n");
			} else {
			  printf("Value: %lu, Protocol: %i", value, mySwitch.getReceivedProtocol());
			  printf("\n");
			}


			mySwitch.resetAvailable();
		}

	}

    return 0;
}

