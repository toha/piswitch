#include "http.h"
#include "../rf/tx.h"
#include "../rf/rx.h"

struct mg_context *webctx;

int http_switch(struct mg_event *event) {

	char post_data[1024];
	int post_data_len;
	post_data_len = mg_read(event->conn, post_data, sizeof(post_data));
	printf("%s\n", post_data);

	json_error_t error;
	json_t* root;
	root = json_loads(&post_data, 0, &error);
	protocol_t* proto;
	proto = (protocol_t*) malloc(sizeof *proto);	

	int ret = json_decode_protocol (proto, root);

	if (0 != ret) {
		fprintf(stderr, "Fehler beim json parsen\n");
		return 0;
	}

	tx_data(proto);

	free(proto);

	char content[100];

	int content_length = snprintf(content, sizeof(content), "ok");

  	mg_printf(event->conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: %d\r\n" // Always set Content-Length
		"\r\n"
		"%s",
		content_length, content);


	return 0;
}


int recCb(unsigned int *a) {
	printf("Callback Fn\n");
	return 0;
}

int http_rec(struct mg_event *event) {
	printf("http rec\n");

	startRecordRfSignal(&recCb);

	char content[100];

	int content_length = snprintf(content, sizeof(content), "ok");

  	mg_printf(event->conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: %d\r\n" // Always set Content-Length
		"\r\n"
		"%s",
		content_length, content);


	return 0;
}



static int event_handler(struct mg_event *event) {

	if (event->type == MG_REQUEST_BEGIN) {
	  
		if (strcmp(event->request_info->uri, "/switch") == 0) {
			return http_switch(event);
		} else if (strcmp(event->request_info->uri, "/rec") == 0) {
			return http_rec(event);
		} else {

			char content[100];
			int content_length = snprintf(content, sizeof(content),
			    "Hello from mongoose! Requested: [%s] [%s]",
			    event->request_info->request_method, event->request_info->uri);

			mg_printf(event->conn,
			    "HTTP/1.1 200 OK\r\n"
			    "Content-Type: text/plain\r\n"
			    "Content-Length: %d\r\n" // Always set Content-Length
			    "\r\n"
			    "%s",
			    content_length, content);

			return 1;
		}
	}

	// We do not handle any other event
	return 0;
}

void startweb(void) {

	const char *options[] = {
		"listening_ports", "8080",
		"num_threads", "5",
		NULL
	};

	// Start the web server.
	webctx = mg_start(options, &event_handler, NULL);
}

void stopweb(void) {
	 mg_stop(webctx);
}
