#include "tx.h"
#include "rx.h"
#include <pthread.h>

#define TX_PIN 0

pthread_mutex_t tx_mutex;

void tx_init() 
{
	pinMode(TX_PIN, OUTPUT);
}

void tx_data (protocol_t * p)
{ 
	pauseRf();
	pthread_mutex_lock(&tx_mutex);
    switch (p->type)
    {
      case 1:
				tx_data_protocol1(&p->p1);
        break;
      case 2:
				tx_data_protocol2(&p->p2);
        break;
      case 3:
				tx_data_protocol3(&p->p3);
        break;
      case 4:
				tx_data_protocol4(&p->p4);
        break;
      case 5:
				tx_data_protocol5(&p->p5);
        break;
      default:
        printf("ERRRRRROR\n");
    		break;
    }

	pthread_mutex_unlock(&tx_mutex);
	resumeRf();
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
