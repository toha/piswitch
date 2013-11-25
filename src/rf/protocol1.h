#ifndef _PROTOCOL1_H_
#define _PROTOCOL1_H_

#include <stdio.h>
#include "../helper.h"

typedef struct
{
	unsigned int network;
	unsigned int address;
	unsigned int state;
} protocol1;

int tx_data_protocol1 (protocol1* self);
protocol1* rx_data_protocol1 (unsigned int timings[], int change_count);
int rx_decode_protocol1(protocol1* self, unsigned long code);
protocol1* json2protocol1 (protocol1* self);

#endif
