#ifndef _PROTOCOL3_H_
#define _PROTOCOL3_H_

#include <stdio.h>
#include "../helper.h"
#include <jansson.h>

typedef struct
{
	unsigned int network;
	unsigned int address;
	unsigned int state;
} protocol3;

int tx_data_protocol3 (protocol3* self);
protocol3* rx_data_protocol3 (unsigned int timings[], int change_count);
int rx_decode_protocol3(protocol3* self, unsigned long code);
int json_decode_protocol3 (protocol3* self, json_t* root);
char* json_encode_protocol3 (protocol3* self);

#endif
