#ifndef _RECEIVE_H_
#define _RECEIVE_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "../lib/rcswitch-pi/RCSwitch.h"
#include "../helper.h"

void recvCallback(unsigned int, unsigned long);
void protocolDecoder(unsigned int, unsigned long);
int startRfReceive(void);


#endif
