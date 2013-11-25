#include "protocol1.h"

#define RX_PROTOCOL1_TOLERANCE 60
#define RX_PROTOCOL1_SYNC_PULSE_RATIO 31

int tx_data_protocol1 (protocol1* self) 
{
	return 0;
}

protocol1* rx_data_protocol1 (unsigned int* timings, int change_count) 
{

  unsigned long code = 0;
  unsigned long delay = timings[0] / RX_PROTOCOL1_SYNC_PULSE_RATIO;
  unsigned long delay_tolerance = delay * RX_PROTOCOL1_TOLERANCE * 0.01;

  for (int i = 1; i<change_count ; i=i+2) {
      if (timings[i] > delay-delay_tolerance 
			 && timings[i] < delay+delay_tolerance 
			 && timings[i+1] > delay*3-delay_tolerance 
			 && timings[i+1] < delay*3+delay_tolerance) {

        code = code << 1;

      } else if (timings[i] > delay*3-delay_tolerance 
							&& timings[i] < delay*3+delay_tolerance 
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
		protocol1* proto;
		proto = (protocol1*) malloc(sizeof *proto);
		int res = rx_decode_protocol1(proto,code);

		return proto;

	} else {
		return NULL;
	}
	
}


int rx_decode_protocol1 (protocol1* self, unsigned long code) 
{
	unsigned int offsetadr = 8;
	self->address = (code & (((1 << 8) - 1) << offsetadr)) >> offsetadr;

	unsigned int offsetnet = 16;
	self->network = (code & (((1 << 8) - 1) << offsetnet)) >> offsetnet;

	unsigned int offsetstate = 0;
	self->state = (code & (((1 << 1) - 1) << offsetstate)) >> offsetstate;

	return 0;
}

protocol1* json2protocol1 (protocol1* self) 
{
	return NULL;
} 
