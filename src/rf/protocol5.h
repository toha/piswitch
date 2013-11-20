#ifndef _PROTOCOL5_H_
#define _PROTOCOL5_H_

#include <stdio.h>

typedef struct
{
	unsigned long network;
	unsigned int address;
	unsigned int broadcast;
	unsigned int state;
	unsigned int dimmer;		
} protocol5;

void tx_data_protocol5 (protocol5* self);


#endif
