#include "rx.h"
#include "protocol.h"
#include "protocol5.h"

#define RX_MAX_INTERRUPT 67
static unsigned int rx_interrupts[RX_MAX_INTERRUPT]; 
static unsigned int rx_irq_duration = 0;
static unsigned int rx_irq_change_count = 0;
static unsigned long rx_irq_lastTime = 0;
static unsigned int rx_irq_repeat_count = 0;
static protocol_t* rx_current_data;
void handleInterrupt (void) { 
	unsigned long time = getMicros();
  rx_irq_duration = time - rx_irq_lastTime;
	//printf("%lu\n", duration);

  if (rx_irq_duration > 5000 && rx_irq_duration > rx_interrupts[0] - 200 && rx_irq_duration < rx_interrupts[0] + 200) {
    rx_irq_repeat_count++;
    rx_irq_change_count--;
    if (rx_irq_repeat_count == 2) {
			protocol5* prot5data;


			prot5data = rx_data_protocol5(rx_interrupts, rx_irq_change_count);
			if (prot5data == NULL) {
				// Nothing found.. Next protocol
			}


			if (prot5data != NULL) {
				
				rx_current_data = (protocol_t*) malloc(sizeof *rx_current_data);
				rx_current_data->type = 5;
				rx_current_data->p5 = *prot5data;

				printf("proto5\n");
				printf("network: %lu\n", prot5data->network);
				printf("address: %d\n", prot5data->address);
				printf("broadcast: %d\n", prot5data->broadcast);
				printf("state: %d\n", prot5data->state);
				printf("dimmer: %d\n\n", prot5data->dimmer);
		
			}



			
      /*if (RCSwitch::receiveProtocol1(change_count) == false){
        if (RCSwitch::receiveProtocol2(change_count) == false){
          if (RCSwitch::receiveProtocol3(change_count) == false){
	          if (RCSwitch::receiveProtocol4(change_count) == false){
		          if (RCSwitch::receiveProtocol5(change_count) == false){
           			
							}
						}
          }
        }
      }*/
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
  if (rx_irq_duration > 150) {
    rx_interrupts[rx_irq_change_count++] = rx_irq_duration;
  }
  rx_irq_lastTime = time; 

}


void recvCallback(unsigned int protocol, unsigned long data)
{
	printf("protocol: %d, value: %lu\n", protocol, data);
	printBits(sizeof(data), &data);

	protocolDecoder(protocol, data);
}

void protocolDecoder(unsigned int protocol, unsigned long data) 
{
	if (protocol == 1) {

	} else if (protocol == 2) {

	} else if (protocol == 3) {

	} else if (protocol == 4) {

	} else if (protocol == 5) {

	}
}

int startRfReceive()
{
	wiringPiSetup();
	wiringPiISR (7, INT_EDGE_BOTH, &handleInterrupt) ;
	/*RCSwitch mySwitch = RCSwitch();
	mySwitch.enableReceive(0);
	mySwitch.fnReceiveCallback = &recvCallback;*/

	while (1) {
		sleep(1);
	}

    return 0;
}


int stopRfReceive()
{
	
	

    return 0;
}

