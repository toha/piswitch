#ifndef _RX_H_
#define _RX_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <wiringPi.h>

#include "../helper.h"


int initRf(void);
int pauseRf(void);
int resumeRf(void);
int stopRfListenForDevices(void);
int startRfListenForDevices(void);
int startRecordRfSignal(int (*rc)(unsigned int[]));


#endif
