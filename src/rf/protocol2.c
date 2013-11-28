#include "protocol2.h"

#define RX_PROTOCOL2_TOLERANCE 60
#define RX_PROTOCOL2_SYNC_PULSE_RATIO 10

int tx_data_protocol2 (protocol2* self) 
{
	return 0;
}

protocol2* rx_data_protocol2 (unsigned int* timings, int change_count) 
{

  unsigned long code = 0;
  unsigned long delay = timings[0] / RX_PROTOCOL2_SYNC_PULSE_RATIO;
  unsigned long delay_tolerance = delay * RX_PROTOCOL2_TOLERANCE * 0.01;

  for (int i = 1; i<change_count ; i=i+2) {
      if (timings[i] > delay-delay_tolerance 
 			 && timings[i] < delay+delay_tolerance 
			 && timings[i+1] > delay*2-delay_tolerance 
			 && timings[i+1] < delay*2+delay_tolerance) {
        code = code << 1;
      } else if (timings[i] > delay*2-delay_tolerance 
							&& timings[i] < delay*2+delay_tolerance 
							&& timings[i+1] > delay-delay_tolerance 
							&& timings[i+1] < delay+delay_tolerance) {
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
		protocol2* proto;
		proto = (protocol2*) malloc(sizeof *proto);
		int res = rx_decode_protocol2(proto,code);

		return proto;

	} else {
		return NULL;
	}
	
}


int rx_decode_protocol2 (protocol2* self, unsigned long code) 
{
	unsigned int offsetadr = 8;
	self->address = (code & (((1 << 8) - 1) << offsetadr)) >> offsetadr;

	unsigned int offsetnet = 16;
	self->network = (code & (((1 << 8) - 1) << offsetnet)) >> offsetnet;

	unsigned int offsetstate = 0;
	self->state = (code & (((1 << 1) - 1) << offsetstate)) >> offsetstate;

	return 0;
}

int json_decode_protocol2 (protocol2* self, json_t* root) 
{
	
	return 0;
} 

/**
 protocol5 2 json
*/
char* json_encode_protocol2 (protocol2* self) 
{

	char* a = "Hallo\0";

	return a;
} 
