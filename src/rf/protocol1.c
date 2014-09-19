#include "protocol1.h"
#include "tx.h"

#define PROTOCOL1_TOLERANCE 60
#define PROTOCOL1_TX_SYNC_PULSE_RATIO 54
#define PROTOCOL1_RX_SYNC_PULSE_RATIO 31
#define PROTOCOL1_BITS_PER_FRAME 24
#define PROTOCOL1_PULSE_LENGTH 185


void tx_1_protocol1 () {
	int pulse_length = PROTOCOL1_PULSE_LENGTH;
	tx_high_low(pulse_length, 5,1);
}

void tx_0_protocol1 () {
	int pulse_length = PROTOCOL1_PULSE_LENGTH;
	tx_high_low(pulse_length, 1,5);
}

void tx_sync_protocol1 () {
	int pulse_length = PROTOCOL1_PULSE_LENGTH;
	tx_low(PROTOCOL1_PULSE_LENGTH * PROTOCOL1_TX_SYNC_PULSE_RATIO);
}
 
int tx_data_protocol1 (protocol1* self) 
{
	unsigned long code = encode_protocol1(self);
	printf("code: %lu\n",code);
	printBits(sizeof(code), &code);
	for (int i=0; i<10; i++) {
		// send sync bit
		tx_sync_protocol1();

		//iterate over bits
		for (int k = PROTOCOL1_BITS_PER_FRAME-1; k>=0; k--) {
			unsigned int bit = (code & ( 1 << k )) >> k;
			if (bit == 1) {
				tx_1_protocol1();
			} else {
				tx_0_protocol1();
			}
		}
		tx_high(PROTOCOL1_PULSE_LENGTH * 1);
	}
	
	// 5 pulses low at the end
	tx_low(PROTOCOL1_PULSE_LENGTH * 5);

	return 0;
}

protocol1* rx_data_protocol1 (unsigned int* timings, int change_count) 
{

  unsigned long code = 0;
  unsigned long delay = timings[0] / PROTOCOL1_RX_SYNC_PULSE_RATIO;
  unsigned long delay_tolerance = delay * PROTOCOL1_TOLERANCE * 0.01;
	
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
		int res = decode_protocol1(proto,code);

		return proto;

	} else {
		return NULL;
	}
}




int decode_protocol1 (protocol1* self, unsigned long code) 
{

	printBits(sizeof(code), &code);
	unsigned int offsetadr = 8;
	self->address = (code & (((1 << 8) - 1) << offsetadr)) >> offsetadr;

	unsigned int offsetnet = 16;
	self->network = (code & (((1 << 8) - 1) << offsetnet)) >> offsetnet;

	unsigned int offsetstate = 0;
	self->state = (code & (((1 << 1) - 1) << offsetstate)) >> offsetstate;

	return 0;
}

unsigned long encode_protocol1 (protocol1* self) 
{
	unsigned long code = 0;
	// startbit
	code += (self->network << 16);
	code += (self->address << 8);
	code += (1 << 4);
	code += (1 << 2);
	code += self->state;

	printBits(sizeof(code), &code);

	return code;

}

/**
	json 2 protocol1
*/
int json_decode_protocol1 (protocol1* self, json_t* root) 
{
  json_error_t error;

	if (!root) {
		  fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		  return 1;
	}

	if (!json_is_object(root)) {
		  fprintf(stderr, "error: root is not an object\n");
			json_decref(root);
			json_decref(root);
		  return 1;
	}

	json_t *network, *address, *state;

  network = json_object_get(root, "network");
  address = json_object_get(root, "address");
  state = json_object_get(root, "state");

  if(!json_is_number(network) ||  !json_is_number(address) || !json_is_number(state)) {
      fprintf(stderr, "error: something is not a number\n");
      json_decref(root);
      return 1;
  }

	self->network = json_number_value(network);
	self->address = json_number_value(address);
	self->state = json_number_value(state);
	
	return 0;
} 

/**
 protocol1 2 json
*/
json_t* json_encode_protocol1 (protocol1* self) 
{
	json_t* root = json_pack("{s:i,s:i,s:i}", 
		"network", self->network,
		"address", self->address,
		"state", self->state
	);
	return root;
	char* s = json_dumps(root, 0);
} 
