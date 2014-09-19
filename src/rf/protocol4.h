#ifndef _PROTOCOL4_H_
#define _PROTOCOL4_H_

#include <stdio.h>
#include "../helper.h"
#include <jansson.h>

typedef struct
{
	unsigned int network;
	unsigned int address;
	unsigned long magic;
} protocol4;

int tx_data_protocol4 (protocol4* self);
protocol4* rx_data_protocol4 (unsigned int timings[], int change_count);
int decode_protocol4(protocol4* self, unsigned long code);
unsigned long encode_protocol4 (protocol4* self);
int json_decode_protocol4 (protocol4* self, json_t* root);
json_t* json_encode_protocol4 (protocol4* self);

#endif
