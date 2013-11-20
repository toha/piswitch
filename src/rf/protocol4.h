#ifndef _PROTOCOL4_H_
#define _PROTOCOL4_H_

typedef struct
{
	unsigned long network;
	unsigned int address;
	unsigned int broadcast;
	unsigned int state;
	unsigned int dimmer;		
} protocol4;

void tx_data_protocol4 (protocol4* data);


#endif
