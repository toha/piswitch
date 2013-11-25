#include "protocol4.h"

#define RX_PROTOCOL4_TOLERANCE 30
#define RX_PROTOCOL4_SYNC_PULSE_RATIO 15

int tx_data_protocol4 (protocol4* self) 
{
	return 0;
}

protocol4* rx_data_protocol4 (unsigned int* timings, int change_count) 
{
	unsigned long code = 1;
	code = code << 1;
	unsigned long delay = timings[0] / RX_PROTOCOL4_SYNC_PULSE_RATIO;
	unsigned long delay_tolerance = delay * RX_PROTOCOL4_TOLERANCE * 0.01; 

  for (int i = 1; i<change_count ; i=i+2) {
    if  (timings[i]   > delay - delay_tolerance 
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
		protocol4* proto;
		proto = (protocol4*) malloc(sizeof *proto);
		int res = rx_decode_protocol4(proto,code);

		return proto;

	} else {
		return NULL;
	}
	
}


int rx_decode_protocol4 (protocol4* self, unsigned long code) 
{

	unsigned int offsetadr = 0;
	self->address = (code & (((1 << 4) - 1) << offsetadr)) >> offsetadr;

	unsigned int offsetnet = 20;
	self->network = (code & (((1 << 3) - 1) << offsetnet)) >> offsetnet;

	unsigned int offsetmag = 4;
	self->magic = (code & (((1 << 16) - 1) << offsetmag)) >> offsetmag;

	return 0;
}

protocol4* json2protocol4 (protocol4* self) 
{
	return NULL;
} 
