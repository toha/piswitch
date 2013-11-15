#include "piswitch.h"

int main()
{
	printf("\nPiSwitch\n");

	startweb();

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
