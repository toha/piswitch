#include "rx.h"
#include "protocol.h"


#define RX_MAX_INTERRUPT 67
#define RX_PIN 7

static unsigned int rx_interrupts[RX_MAX_INTERRUPT]; 
static unsigned int rx_irq_duration = 0;
static unsigned int rx_irq_change_count = 0;
static unsigned long rx_irq_lastTime = 0;
static unsigned int rx_irq_repeat_count = 0;
static protocol_t* rx_current_data;


void handleInterrupt (void) { 
	unsigned long time = getMicros();
  rx_irq_duration = time - rx_irq_lastTime;

  if (rx_irq_duration > 5000 && rx_irq_duration > rx_interrupts[0] - 200 && rx_irq_duration < rx_interrupts[0] + 200) {
    rx_irq_repeat_count++;
    rx_irq_change_count--;
    if (rx_irq_repeat_count == 2) {
		protocol5* prot5data = NULL;
		protocol4* prot4data = NULL;
		protocol1* prot1data = NULL;
		protocol3* prot3data = NULL;
		protocol2* prot2data = NULL;

		bool found_something = true;
		
		prot5data = rx_data_protocol5(rx_interrupts, rx_irq_change_count);
		if (prot5data == NULL) {
			prot4data = rx_data_protocol4(rx_interrupts, rx_irq_change_count);
			if (prot4data == NULL) {
				prot1data = rx_data_protocol1(rx_interrupts, rx_irq_change_count);
				if (prot1data == NULL) {
					prot3data = rx_data_protocol3(rx_interrupts, rx_irq_change_count);
					if (prot3data == NULL) {
						prot2data = rx_data_protocol2(rx_interrupts, rx_irq_change_count);
						if (prot2data == NULL) {
							// Nothing found.. 
							found_something = false;
						}
					}
				}
			}
		}

		if (found_something) {
			rx_current_data = (protocol_t*) malloc(sizeof *rx_current_data);
		}

		if (prot5data != NULL) {
			rx_current_data->type = 5;
			rx_current_data->p5 = *prot5data;

			//tx_data_protocol5(prot5data);

		} else if (prot4data != NULL) {
			rx_current_data->type = 4;
			rx_current_data->p4 = *prot4data;
		} else if (prot1data != NULL) {
			rx_current_data->type = 1;
			rx_current_data->p1 = *prot1data;

		} else if (prot3data != NULL) {
			rx_current_data->type = 3;
			rx_current_data->p3 = *prot3data;
		} else if (prot2data != NULL) {
			rx_current_data->type = 2;
			rx_current_data->p2 = *prot2data;
		}

		if (found_something) {
			printf("etwas empfangen\n\n");
			printProtocol(rx_current_data);

		}
		

		rx_irq_repeat_count = 0;

    }

    rx_irq_change_count = 0;

  } else if (rx_irq_duration > 5000) {
    rx_irq_change_count = 0;
  }
 
  if (rx_irq_change_count >= RX_MAX_INTERRUPT) {
    rx_irq_change_count = 0;
    rx_irq_repeat_count = 0;
  }
  if (rx_irq_duration > 50) {
    rx_interrupts[rx_irq_change_count++] = rx_irq_duration;
  }
  rx_irq_lastTime = time; 

}


void recvCallback(unsigned int protocol, unsigned long data)
{
	printf("protocol: %d, value: %lu\n", protocol, data); 
	printBits(sizeof(data), &data);
}


int startRfReceive()
{
	//wiringPiSetup();
	wiringPiISR (RX_PIN, INT_EDGE_BOTH, &handleInterrupt) ;

	while (1) {
		sleep(1);
	}

    return 0;
}


int stopRfReceive()
{
	
	

    return 0;
}

