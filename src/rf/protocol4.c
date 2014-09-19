#include "protocol4.h"

#define PROTOCOL4_TOLERANCE 60
#define PROTOCOL4_SYNC_PULSE_RATIO 15
#define PROTOCOL4_BITS_PER_FRAME 24
#define PROTOCOL4_PULSE_LENGTH 456

void tx_1_protocol4 () {
	int pulse_length = PROTOCOL4_PULSE_LENGTH;
	tx_high_low(pulse_length, 2,1);
}

void tx_0_protocol4 () {
	int pulse_length = PROTOCOL4_PULSE_LENGTH;
	tx_high_low(pulse_length, 1,2);
}

void tx_sync_protocol4 () {
	/*tx_high(6 * 482);
	tx_low(PROTOCOL4_SYNC_PULSE_RATIO * 465);*/
	tx_high(6 * 500);
	tx_low(14 * 500);
}

int tx_data_protocol4 (protocol4* self) 
{
	unsigned long code = encode_protocol4(self);

	for (int i=0; i<10; i++) {

		//iterate over bits
		for (int k = PROTOCOL4_BITS_PER_FRAME-1; k>=0; k--) {

			unsigned int bit = (code & ( 1 << k )) >> k;
				//printf("bit %d: %d\n", k,bit);		
			if (bit == 1) {
				tx_1_protocol4();
			} else {
				tx_0_protocol4();
			}
		}

		tx_sync_protocol4();

		//tx_high(1);		
	}


	return 0;
}

protocol4* rx_data_protocol4 (unsigned int* timings, int change_count) 
{
	unsigned long code = 1;
	code = code << 1;
	unsigned long delay = timings[0] / PROTOCOL4_SYNC_PULSE_RATIO;
	unsigned long delay_tolerance = delay * PROTOCOL4_TOLERANCE * 0.01; 

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
		int res = decode_protocol4(proto,code);

		return proto;

	} else {
		return NULL;
	}
	
}


int decode_protocol4 (protocol4* self, unsigned long code) 
{
printBits(sizeof(code), &code);
	unsigned int offsetadr = 0;
	self->address = (code & (((1 << 4) - 1) << offsetadr)) >> offsetadr;

	unsigned int offsetnet = 20;
	self->network = (code & (((1 << 3) - 1) << offsetnet)) >> offsetnet;

	unsigned int offsetmag = 4;
	self->magic = (code & (((1 << 16) - 1) << offsetmag)) >> offsetmag;

	return 0;
}


unsigned long encode_protocol4 (protocol4* self) 
{
	unsigned long code = 0;
	//code += (1 << 24);
	code += (self->network << 20);
	code += (self->magic << 4);
	code += self->address;

	//printBits(sizeof(code), &code);

	return code;
}


/**
	json 2 protocol4
*/
int json_decode_protocol4 (protocol4* self, json_t* root) 
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

	json_t *network, *address, *magic;

  network = json_object_get(root, "network");
  address = json_object_get(root, "address");
  magic = json_object_get(root, "magic");

  if(!json_is_number(network) ||  !json_is_number(address) || !json_is_number(magic)) {
      fprintf(stderr, "error: something is not a number\n");
      json_decref(root);
      return 1;
  }

	self->network = json_number_value(network);
	self->address = json_number_value(address);
	self->magic = json_number_value(magic);
	
	return 0;
} 

/**
 protocol4 2 json
*/
json_t* json_encode_protocol4 (protocol4* self) 
{
	json_t* root = json_pack("{s:i,s:i,s:i}", 
		"network", self->network,
		"address", self->address,
		"magic", self->magic
	);
	return root;
	char* s = json_dumps(root, 0);
} 
