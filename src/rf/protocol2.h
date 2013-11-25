#ifndef _PROTOCOL2_H_
#define _PROTOCOL2_H_

#include <stdio.h>
#include "../helper.h"

typedef struct
{
	unsigned int network;
	unsigned int address;
	unsigned int state;
} protocol2;

int tx_data_protocol2 (protocol2* self);
protocol2* rx_data_protocol2 (unsigned int timings[], int change_count);
int rx_decode_protocol2(protocol2* self, unsigned long code);
protocol2* json2protocol2 (protocol2* self);

#endif
