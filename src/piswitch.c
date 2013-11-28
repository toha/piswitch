#include "piswitch.h"
#include <wiringPi.h>
#include <jansson.h>

int main()
{
	printf("\nPiSwitch\n");

	wiringPiSetup () ;
	tx_init();
	startweb();

	int recpid;
	recpid = fork();
	if (recpid == 0) {
		// child
		startRfReceive();


	} else if (recpid > 0) { 
		// parent
		
		/*protocol_t* test1;
		test1 = (protocol_t*) malloc(sizeof *test1);
		if (test1 != NULL) {
			test1->type = PROTOCOL5;
			protocol5* b;
			b = (protocol5*) malloc(sizeof *b);

			b->network = 2716;
			b->address = 2;
			b->broadcast = 0;
			b->state = 0;
			b->dimmer = 0;

			test1->p5 = *b;
			for (int i=0; i<3; i++) {
				printf("senden\n");
				tx_data(test1);
				delayMicroseconds(1000*10);
			}

			free(b);
		}

		free(test1);*/



		/*char* jsontest = "{\r\n"
		  "\"network\": 2115,\r\n"
		  "\"address\": 2,\r\n"
		  "\"broadcast\": 1,\r\n"
		  "\"state\": 1,\r\n"
		  "\"dimmer\": 1\r\n"
		  "}";
		protocol5* b;
		b = (protocol5*) malloc(sizeof *b);
		int ret = json_decode_protocol5 (b, jsontest);
		if (0 != ret) {
			fprintf(stderr, "Fehler beim json parsen\n");
		} else {
			printf("network: %d\n", b->network);
			printf("address: %d\n", b->address);
			printf("broadcast: %d\n", b->broadcast);
			printf("state: %d\n", b->state);
			printf("dimmer: %d\n", b->dimmer);
		}*/


		char* jsontest = 
			"{\r\n"
			  "\"type\": 5,\r\n"
			  "\"p1\": null,\r\n"
			  "\"p2\": null,\r\n"
			  "\"p3\": null,\r\n"
			  "\"p4\": null,\r\n"
			  "\"p5\": {\r\n"
				  "\"network\": 2115,\r\n"
				  "\"address\": 2,\r\n"
				  "\"broadcast\": 1,\r\n"
				  "\"state\": 1,\r\n"
				  "\"dimmer\": 1\r\n"
			  "}"
			"}";

		json_error_t error;
		json_t* root;
		root = json_loads(jsontest, 0, &error);

		protocol_t* a;
		a = (protocol_t*) malloc(sizeof *a);
		int ret = json_decode_protocol (a, root);

		if (0 != ret) {
			fprintf(stderr, "Fehler beim json parsen\n");
		} else {
			printf("type: %d\n", a->type);
			printf("network: %d\n", a->p5.network);
			printf("address: %d\n", a->p5.address);
			printf("broadcast: %d\n", a->p5.broadcast);
			printf("state: %d\n", a->p5.state);
			printf("dimmer: %d\n", a->p5.dimmer);

			json_t* r = json_encode_protocol(a);

			char* s = json_dumps(r, 0);
				if (s == NULL) {
					printf("fehler\n");
				}
				else {
					printf("%s\n", s );	
				}

		}




		while(1) {
			sleep(1);
		}

		
	} else {   
	  fprintf (stderr, "Error");
	  exit (1);
	}

	return 0;
}
