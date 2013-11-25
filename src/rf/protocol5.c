#include "protocol5.h"

#define RX_PROTOCOL5_TOLERANCE 60
#define RX_PROTOCOL5_PULSE_LENGTH 670

int tx_data_protocol5 (protocol5* self) 
{
	printf("proto5\n");
	printf("network: %lu\n", self->network);
	printf("address: %d\n", self->address);
	printf("broadcast: %d\n", self->broadcast);
	printf("state: %d\n", self->state);
	return 0;
}

protocol5* rx_data_protocol5 (unsigned int* timings, int change_count) 
{
	unsigned long code = 1;
	code = code << 1;
	unsigned long delay = RX_PROTOCOL5_PULSE_LENGTH;
	unsigned long delay_tolerance = delay * RX_PROTOCOL5_TOLERANCE * 0.01; 

	for (int i = 2; i<change_count ; i=i+2) {
		if (timings[i]   > delay - delay_tolerance 
		 && timings[i]   < delay + delay_tolerance 
		 && timings[i+1] > delay*2  - delay_tolerance 
		 && timings[i+1] < delay*2  + delay_tolerance) { 

			code = code << 1;

		} else if (timings[i]   > delay*2 - delay_tolerance
				&& timings[i]   < delay*2 + delay_tolerance
				&& timings[i+1] > delay - delay_tolerance
				&& timings[i+1] < delay + delay_tolerance) {

			code+=1;
			code = code << 1;

		} else {
			// Failed
			i = change_count;
			code = 0;
		}
	}      
	code = code >> 1;
	if (change_count/2 >= 15 && code != 0) {
		protocol5* proto;
		proto = (protocol5*) malloc(sizeof *proto);
		int res = rx_decode_protocol5(proto,code);

		return proto;

	} else {
		return NULL;
	}
	
}


int rx_decode_protocol5 (protocol5* self, unsigned long code) 
{
	unsigned int offsetnet = 8;
	self->network = (code & (((1 << 12) - 1) << offsetnet)) >> offsetnet;

	unsigned int offsetadr = 6;
	self->address = (code & (((1 << 2) - 1) << offsetadr)) >> offsetadr;

	unsigned int offsetbr = 5;
	self->broadcast = (code & (((1 << 1) - 1) << offsetbr)) >> offsetbr;

	unsigned int offsetonoff = 4;
	self->state = (code & (((1 << 1) - 1) << offsetonoff)) >> offsetonoff;

	unsigned int offsetdi = 3;
	self->dimmer = (code & (((1 << 1) - 1) << offsetdi)) >> offsetdi;		

	return 0;
}

protocol5* json2protocol5 (protocol5* self) 
{
	return NULL;
} 
