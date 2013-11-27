#include "piswitch.h"
#include <wiringPi.h>
#include <sys/ipc.h>; 
#include <sys/msg.h>; 

int main()
{
	printf("\nPiSwitch\n");

	startweb();

	wiringPiSetup () ;
	tx_init();

	int msqid = msgget(4, (IPC_CREAT | IPC_EXCL | 0400));
	printf("msqid: %d\n", msqid);

	int recpid;
	recpid = fork();
	if (recpid == 0) {
		// child
		startRfReceive();
	} else if (recpid > 0) { 
		// parent

		
		protocol_t* test1;
		test1 = (protocol_t*) malloc(sizeof *test1);
		if (test1 != NULL) {
			test1->type = PROTOCOL5;
			protocol5* b;
			b = (protocol5*) malloc(sizeof *b);

			b->network = 2716;
			b->address = 2;
			b->broadcast = 0;
			b->state = 1;
			b->dimmer = 0;

			test1->p5 = *b;

			for (int i=0; i<3; i++) {
				printf("senden\n");
				tx_data(test1);
				delayMicroseconds(1000*10);
			}

			free(b);
		}

		free(test1);

		while(1) {
			sleep(1);
		}

		
	} else {   
	  fprintf (stderr, "Error");
	  exit (1);
	}

	return 0;
}
