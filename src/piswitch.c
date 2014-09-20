#include "piswitch.h"
#include <wiringPi.h>
#include <jansson.h>


int testFn(protocol_t* rxdata) {
	printf("Observer in piswitch\n");
	printProtocol(rxdata);
	//free(rxdata);
}

int main()
{
	printf("\nPiSwitch - pid: %d\n", getpid() );

	wiringPiSetup () ;
	tx_init();
	startweb();

	printf("call initRf - pid: %d\n", getpid() );
	initRf();

	//registerDeviceObserver(&testFn); 

	while(1) {
		sleep(1);
	}	

	/*int recpid;
	recpid = fork();
	if (recpid == 0) {
		// child
		printf("call initRf - pid: %d\n", getpid() );
		initRf();

		//startRecordRfSignal(&testFn);

		while (1) {
			sleep(1);
		}	

	} else if (recpid > 0) { 
		// parent 
		


		while(1) {
			sleep(1);
		}

	} else {   
	  fprintf (stderr, "Error");
	  exit (1);
	}*/

	return 0;
}
