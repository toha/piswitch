#ifndef _PROTOCOL2_H_
#define _PROTOCOL2_H_


typedef struct
{
	unsigned long network;
	unsigned int address;
	unsigned int broadcast;
	unsigned int state;
	unsigned int dimmer;		
} protocol2;

void tx_data_protocol2 (protocol2* data);


#endif
