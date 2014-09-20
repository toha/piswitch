#ifndef _RX_H_
#define _RX_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <wiringPi.h>

#include "../helper.h"


#define RX_MAX_INTERRUPT 67
#define RX_PIN 7

#define RX_REC_DATA_SIZE 8192
#define RX_MODE_DEFAULT 1

#define RX_OBSERVER_MAX 100

int initRf(void);
int registerDeviceObserver(int (*rxDeviceObserver)());
int removeDeviceObserver(int idx);
int pauseRf(void);
int resumeRf(void);
int stopRfListenForDevices(void);
int startRfListenForDevices(void);
int startRecordRfSignal(int (*rc)(unsigned int[]));


#endif
