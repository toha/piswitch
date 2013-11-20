#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "protocol1.h"
#include "protocol2.h"
#include "protocol3.h"
#include "protocol4.h"
#include "protocol5.h"

enum
{
	PROTOCOL1,
	PROTOCOL2,
	PROTOCOL3,
	PROTOCOL4,
	PROTOCOL5
};


typedef struct
{
    int type;
    protocol1 p1;
    protocol2 p2;
    protocol3 p3;
    protocol4 p4;
    protocol5 p5;
} protocol_t;


#endif
