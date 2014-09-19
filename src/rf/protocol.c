#include "protocol.h"


/**
	json 2 protocol5
*/
int json_decode_protocol (protocol_t* self, json_t* root) 
{
  json_error_t error;

	if (!root) {
	  fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
	  return 1;
	}

	if (!json_is_object(root)) {
	  fprintf(stderr, "error: root is not an object\n");
		json_decref(root);
	  return 1;
	}

	json_t *type, *json_p1, *json_p2, *json_p3, *json_p4, *json_p5;

  type = json_object_get(root, "type");
  json_p1 = json_object_get(root, "p1");
  json_p2 = json_object_get(root, "p2");
  json_p3 = json_object_get(root, "p3");
  json_p4 = json_object_get(root, "p4");
  json_p5 = json_object_get(root, "p5");

  if(!json_is_number(type)) {
    fprintf(stderr, "error: something is not a there\n");
    json_decref(root);
    return 1;
  }

  if (json_is_null(json_p1) && json_is_null(json_p2) && json_is_null(json_p3) && 
		json_is_null(json_p4) && json_is_null(json_p5)) {
    fprintf(stderr, "error: one protocol must be defined\n");
    json_decref(root);
    return 1;
  }

	self->type = json_number_value(type);

	if (self->type == 1) {
		printf("proto1!!!\n");
		protocol1* p1;
		p1 = (protocol1*) malloc(sizeof *p1);
		int p1ret = json_decode_protocol1 (p1, json_p1);
		self->p1 = *p1;
	} else if (self->type == 4) {
		protocol4* p4;
		p4 = (protocol4*) malloc(sizeof *p4);
		int p4ret = json_decode_protocol4 (p4, json_p4);
		self->p4 = *p4;
	} else if (self->type == 5) {
		protocol5* p5;
		p5 = (protocol5*) malloc(sizeof *p5);
		int p5ret = json_decode_protocol5 (p5, json_p5);
		self->p5 = *p5;
	}
	
	return 0;
}

json_t* json_encode_protocol (protocol_t* self) 
{

	json_t* root = json_pack("{s:i}", "type", self->type);

	json_t* p1;
	if (self->type != 1) {
		p1 = json_null();
	} else {
		p1 = json_encode_protocol1(&self->p1);
	}
	json_object_set(root, "p1", p1);

	json_t* p2;
	if (self->type != 2) {
		p2 = json_null();
	} else {
		p2 = json_encode_protocol2(&self->p2);
	}
	json_object_set(root, "p2", p2);

	json_t* p3;
	if (self->type != 3) {
		p3 = json_null();
	} else {
		p3 = json_encode_protocol3(&self->p3);
	}
	json_object_set(root, "p3", p3);

	json_t* p4;
	if (self->type != 4) {
		p4 = json_null();
	} else {
		p4 = json_encode_protocol4(&self->p4);
	}
	json_object_set(root, "p4", p4);

	json_t* p5;
	if (self->type != 5) {
		p5 = json_null();
	} else {
		p5 = json_encode_protocol5(&self->p5);
	}
	json_object_set(root, "p5", p5);

	return root;
} 

void printProtocol(protocol_t* proto)
{
	printf("type: %d\n", proto->type);

	if (proto->type == 1) {
		printf("network: %d\n", proto->p1.network);
		printf("address: %d\n", proto->p1.address);
		printf("state: %d\n", proto->p1.state);
	} else if(proto->type == 2) {

	} else if(proto->type == 3) {

	} else if(proto->type == 4) {
		printf("network: %d\n", proto->p4.network);
		printf("address: %d\n", proto->p4.address);
		printf("magic: %d\n", proto->p4.magic);	
	} else if(proto->type == 5) {
		printf("network: %d\n", proto->p5.network);
		printf("address: %d\n", proto->p5.address);
		printf("broadcast: %d\n", proto->p5.broadcast);
		printf("state: %d\n", proto->p5.state);
		printf("dimmer: %d\n", proto->p5.dimmer);	
	}


}
