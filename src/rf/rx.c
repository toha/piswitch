#include "rx.h"
#include "protocol.h"

static unsigned int rx_interrupts[RX_MAX_INTERRUPT]; 
static unsigned int rx_irq_duration = 0;
static unsigned int rx_irq_change_count = 0;
static unsigned long rx_irq_lastTime = 0;
static unsigned int rx_irq_repeat_count = 0;
static protocol_t* rx_current_data;


// 0: do not receive
// 1: permanent receive
// 2: record signal
static unsigned int rx_mode = RX_MODE_DEFAULT;
static unsigned int rx_mode_saved = 0;
static int (*recordingCallback)(unsigned int[]);
static unsigned int rec_cursor = 0;
static unsigned int *rec_data = NULL;

static int (*rxDeviceObserverList[RX_OBSERVER_MAX])(protocol_t*);

int registerDeviceObserver(int (*rxDeviceObserver)(protocol_t*)) {
	for (int i=0; i<RX_OBSERVER_MAX; i++) {
		if(rxDeviceObserverList[i] == NULL) {
			rxDeviceObserverList[i] = rxDeviceObserver;
			return i;
		}
	}
	return -1;
}

int removeDeviceObserver(int idx) {
	rxDeviceObserverList[idx] = NULL;
}

int stopRfListenForDevices()
{
	if (rx_mode == 2) {
		printf("Cant stop listening for devices while recording\n");
		return -1;
	}
	rx_mode = 0;
    return 0;
}

int startRfListenForDevices()
{
	if (rx_mode == 2) {
		printf("Cant start listening for devices while recording\n");
		return -1;
	}	
	rx_mode = 1;
    return 0;
}

void handleInterrupt(void) { 
	//printf("handleInterrupt() rx_mode: %d    pid: %d\n", rx_mode, getpid());
    switch (rx_mode)
    {
      case 0:
				return;
        		break;
      case 1:
				rxListenForDevicesInterruptHandler();
        		break;
      case 2:
				recordRfSignalInterruptHandler();
        		break;
      default:
        printf("Unkown RX mode, Aborting!\n");
    	break;
    }
}




void rxListenForDevicesInterruptHandler() {
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
				printf("\nProtocol received and recognized\n");
				//printProtocol(rx_current_data);

				for (int i=0; i<RX_OBSERVER_MAX; i++) {
					if(rxDeviceObserverList[i] != NULL) {
						(*rxDeviceObserverList[i])(rx_current_data);
					}
				}			

			}
		
			rx_irq_repeat_count = 0;

		}

		rx_irq_change_count = 0;

	} 
	else if (rx_irq_duration > 5000) {
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


void recordRfSignalInterruptHandler() 
{	
	if (rec_cursor >= RX_REC_DATA_SIZE-1) {
		completeRecording();
		return;
	}


	unsigned long time = getMicros();
	rx_irq_duration = time - rx_irq_lastTime;

	rec_data[rec_cursor] = rx_irq_duration;
	rec_cursor++;
	//printf("rec duration %d\n", rx_irq_duration);

	rx_irq_lastTime = time;
}

void completeRecording() {
	printf("Complete Recordin\n");
	rec_cursor = 0;
	recordingCallback(rec_data);
	rx_mode = RX_MODE_DEFAULT;

}

/**
* Async!
*/
int startRecordRfSignal(int (*rc)(unsigned int[]))
{
	if (rx_mode == 2) {
		printf("Recording already running\n");
		return -1;
	}	

	rec_data = malloc(RX_REC_DATA_SIZE*sizeof(unsigned int));
	if( NULL == rec_data ) {
		printf("Fehler bei malloc....\n");
		return -1;
	}

	rec_cursor = 0;
	recordingCallback = rc;
	rx_mode = 2;

	return 0;
}


int resumeRf()
{
	rx_mode = rx_mode_saved;
	return 0;
}

int pauseRf()
{
	rx_mode_saved = rx_mode;
	rx_mode = 0;
    return 0;
}

int initRf()
{
	printf("initRf - pid: %d\n", getpid() );
	wiringPiISR (RX_PIN, INT_EDGE_BOTH, &handleInterrupt) ;
    return 0;
}