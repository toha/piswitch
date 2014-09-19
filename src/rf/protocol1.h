#ifndef _PROTOCOL1_H_
#define _PROTOCOL1_H_

#include <stdio.h>
#include "../helper.h"
#include <jansson.h>

typedef struct
{
	unsigned int network;
	unsigned int address;
	unsigned int state;
} protocol1;

int tx_data_protocol1 (protocol1* self);
protocol1* rx_data_protocol1 (unsigned int timings[], int change_count);
int decode_protocol1(protocol1* self, unsigned long code);
unsigned long encode_protocol1 (protocol1* self);
int json_decode_protocol1 (protocol1* self, json_t* root);
json_t* json_encode_protocol1 (protocol1* self);

#endif
