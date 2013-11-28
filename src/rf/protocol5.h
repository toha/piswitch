#ifndef _PROTOCOL5_H_
#define _PROTOCOL5_H_

#include <stdio.h>
#include "../helper.h"
#include <jansson.h>

typedef struct
{
	unsigned long network;
	unsigned int address;
	unsigned int broadcast;
	unsigned int state;
	unsigned int dimmer;		
} protocol5;

int tx_data_protocol5 (protocol5* self);
protocol5* rx_data_protocol5 (unsigned int timings[], int change_count);

void tx_1_protocol5 ();
void tx_0_protocol5 ();
void tx_sync_protocol5();

int decode_protocol5(protocol5* self, unsigned long code);
unsigned long encode_protocol5 (protocol5* self);
unsigned int calc_checksum_protocol5(unsigned int payload);

int json_decode_protocol5 (protocol5* self, json_t* root);
json_t* json_encode_protocol5 (protocol5* self);

#endif
