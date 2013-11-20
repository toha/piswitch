#include "protocol5.h"

void tx_data_protocol5 (protocol5* self) 
{

	
	printf("proto5\n");
	printf("network: %lu\n", self->network);
	printf("address: %d\n", self->address);
	printf("broadcast: %d\n", self->broadcast);
	printf("state: %d\n", self->state);
}


protocol5* json2protocol5 (protocol5* self) {
	return NULL;
} 
