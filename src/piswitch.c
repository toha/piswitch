#include "piswitch.h"

int main()
{
	printf("\nPiSwitch\n");

	startweb();

	protocol_t *test1;
	test1 = malloc(sizeof *test1);
	if (test1 != NULL) {
		test1->type = PROTOCOL5;
		protocol5 *b;
		b = malloc(sizeof *b);

		b->network = 999;
		b->address = 1;
		b->broadcast = 0;
		b->state = 1;
		b->dimmer = 0;

		test1->p5 = *b;

		tx_data(test1);

		free(b);
	}

	free(test1);


	int recpid;
	recpid = fork();
	if (recpid == 0) {
		// child
		startRfReceive();
	} else if (recpid > 0) { 
		// parent
		while(1) {
			sleep(1);
		}
	} else {   
	  fprintf (stderr, "Error");
	  exit (1);
	}

	return 0;
}
