#ifndef _PROTOCOL5_H_
#define _PROTOCOL5_H_

#include <stdio.h>
#include "../helper.h"

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
static int rx_decode_protocol5(protocol5* self, unsigned long code);

#endif
