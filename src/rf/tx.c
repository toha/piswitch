#include "tx.h"
#include <pthread.h>

#define TX_PIN 0

pthread_mutex_t tx_mutex;

void tx_init() 
{
	pinMode(TX_PIN, OUTPUT);
}

void tx_data (protocol_t * p)
{ 
    switch (p->type)
    {
        case PROTOCOL1:
			tx_data_protocol1(&p->p1);
            break;
        case PROTOCOL2:
			tx_data_protocol2(&p->p2);
            break;
        case PROTOCOL3:
			tx_data_protocol3(&p->p3);
            break;
        case PROTOCOL4:
			tx_data_protocol4(&p->p4);
            break;
        case PROTOCOL5:
			tx_data_protocol5(&p->p5);
            break;
        default:
            printf("ERRRRRROR\n");
            break;
    }
}

void tx_data_n_times (protocol_t * p, int n)
{ 
	for (int i=0; i<n; i++) {
		printf("senden: %d\n", i);
		tx_data(p);
		delayMicroseconds(1000*5*1000);
	}
}

void tx_high_low(int pulse_length, int num_high_pulses, int num_low_pulses) {

	int pin = TX_PIN;
	if (num_high_pulses > 0) {
		digitalWrite(pin, HIGH);
		delayMicroseconds( pulse_length * num_high_pulses);
	}
	if (num_low_pulses > 0) {
		digitalWrite(pin, LOW);
		delayMicroseconds( pulse_length * num_low_pulses);
	}
}

void tx_low_high(int pulse_length, int num_low_pulses, int num_high_pulses) {

	int pin = TX_PIN;
	if (num_low_pulses > 0) {
		digitalWrite(pin, LOW);
		delayMicroseconds( pulse_length * num_low_pulses);
	}
	if (num_high_pulses > 0) {
		digitalWrite(pin, HIGH);
		delayMicroseconds( pulse_length * num_high_pulses);
	}
}

void tx_low(int pulse_length) {
	int pin = TX_PIN;
	digitalWrite(pin, LOW);
	delayMicroseconds(pulse_length);
}

void tx_high(int pulse_length) {
	int pin = TX_PIN;
	digitalWrite(pin, HIGH);
	delayMicroseconds(pulse_length);
}
