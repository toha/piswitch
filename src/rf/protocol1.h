#ifndef _PROTOCOL1_H_
#define _PROTOCOL1_H_

typedef struct
{
	unsigned long network;
	unsigned int address;
	unsigned int broadcast;
	unsigned int state;
	unsigned int dimmer;		
} protocol1;

void tx_data_protocol1 (protocol1* data);


#endif
