#ifndef _TX_H_
#define _TX_H_

#include <stdio.h>
#include "protocol.h"
#include <wiringPi.h>

void tx_init();
void tx_data (protocol_t * p);
void tx_high_low(int pulse_length, int num_high_pulses, int num_low_pulses);
void tx_low_high(int pulse_length, int num_low_pulses, int num_high_pulses);
void tx_low(int pulse_length);
void tx_high(int pulse_length);

#endif
