#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "protocol1.h"
#include "protocol2.h"
#include "protocol3.h"
#include "protocol4.h"
#include "protocol5.h"
#include <jansson.h>


typedef struct
{
    int type;
    protocol1 p1;
    protocol2 p2;
    protocol3 p3;
    protocol4 p4;
    protocol5 p5;
} protocol_t;

int json_decode_protocol (protocol_t* self, json_t* root);
json_t* json_encode_protocol (protocol_t* self);
#endif
