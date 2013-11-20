#ifndef _PROTOCOL3_H_
#define _PROTOCOL3_H_


typedef struct
{
	unsigned long network;
	unsigned int address;
	unsigned int broadcast;
	unsigned int state;
	unsigned int dimmer;		
} protocol3;

void tx_data_protocol3 (protocol3* data);


#endif
