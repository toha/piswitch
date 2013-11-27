#include "protocol5.h"
#include "tx.h"

#define PROTOCOL5_TOLERANCE 40
#define PROTOCOL5_PULSE_LENGTH 670
#define PROTOCOL5_BITS_PER_FRAME 21

int tx_data_protocol5 (protocol5* self) 
{
	unsigned long code = encode_protocol5(self);
	
	// send sync bit
	tx_sync_protocol5();

	//iterate over bits
	for (int k = PROTOCOL5_BITS_PER_FRAME-1; k>=0; k--) {

		unsigned int bit = (code & ( 1 << k )) >> k;
		if (bit == 1) {
			tx_1_protocol5();
		} else {
			tx_0_protocol5();
		}
	}

	// 5 pulses low at the end
	tx_low(PROTOCOL5_PULSE_LENGTH * 5);

	return 0;
}

void tx_1_protocol5 () {
	int pulse_length = PROTOCOL5_PULSE_LENGTH;
	tx_low_high(pulse_length, 2,1);
}

void tx_0_protocol5 () {
	int pulse_length = PROTOCOL5_PULSE_LENGTH;
	tx_low_high(pulse_length, 1,2);
}

void tx_sync_protocol5 () {

	tx_low(PROTOCOL5_PULSE_LENGTH * 2);
	tx_high(PROTOCOL5_PULSE_LENGTH * 2);    
	tx_low(PROTOCOL5_PULSE_LENGTH * 10);
}

protocol5* rx_data_protocol5 (unsigned int* timings, int change_count) 
{
	unsigned long code = 1;
	code = code << 1;
	unsigned long delay = PROTOCOL5_PULSE_LENGTH;
	unsigned long delay_tolerance = delay * PROTOCOL5_TOLERANCE * 0.01; 
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
		decode_protocol5(proto,code);

		return proto;

	} else {
		return NULL;
	}
}


int decode_protocol5 (protocol5* self, unsigned long code) 
{
	printf("\n\ndecode\n");
	printBits(sizeof(code), &code);
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

unsigned long encode_protocol5 (protocol5* self) 
{
	unsigned long code = 0;
	// startbit
	code += 1;
	code = code << 20;

	code += (self->network << 8);
	code += (self->address << 6);
	code += (self->broadcast << 5);
	code += (self->state << 4);
	code += (self->dimmer << 3);

	unsigned int payload = 0;
	payload = (code & 0b11111100) >> 2;
	unsigned int checksum = calc_checksum_protocol5(payload);
	code += checksum;
	
	return code;

}

unsigned int calc_checksum_protocol5(unsigned int payload) {
  unsigned long button = (payload & 0b00110000) >> 4;
  unsigned long broadcastAndOnOff = (payload & 0b00001100) >> 2;
  unsigned long dimmerAndMystery = payload & 0b00000011;
  unsigned long cs = (button ^ broadcastAndOnOff) ^ dimmerAndMystery;
 
  return cs; 
}

protocol5* json2protocol5 (protocol5* self) 
{
	return NULL;
} 
