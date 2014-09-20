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

int http_rec_rx_data(struct mg_event *event) {

	printf("http rec - pid: %d\n", getpid());

	pthread_mutex_t rec_mutex;
	pthread_mutex_lock(&rec_mutex);

	int recCb(unsigned int *rec_data) {
		printf("Callback Fn\n");

		json_t* root = json_pack("{s:s,s:[]}", 
			"type", "recording",
			"data"
		);
		json_t* jsondata = json_object_get(root, "data");
		for (int i=0; i<RX_REC_DATA_SIZE; i++) {
			json_array_append(jsondata, json_integer(rec_data[i]));
		}

		char* json_str = json_dumps(root, 0);

		int content_length = strlen(json_str);
	  	mg_printf(event->conn,
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: %d\r\n"
			"\r\n"
			"%s",
			content_length, json_str);

	  	free(rec_data);
	  	free(root);
	  	free(json_str);
	  	pthread_mutex_unlock(&rec_mutex);
		return 0;
	};

	startRecordRfSignal(&recCb);

	pthread_mutex_lock(&rec_mutex);
	pthread_mutex_unlock(&rec_mutex);
	printf("return http_rec\n");
	return 0;
}


int http_rx_device_protocol(struct mg_event *event) {

	printf("http rec - pid: %d\n", getpid());

	pthread_mutex_t rec_mutex;
	pthread_mutex_lock(&rec_mutex);

	int recCb(protocol_t* rxdata) {
		printf("Observer in http\n");

		json_t* root = json_encode_protocol(rxdata);

		char* json_str = json_dumps(root, 0);

		int content_length = strlen(json_str);
	  	mg_printf(event->conn,
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: %d\r\n"
			"\r\n"
			"%s",
			content_length, json_str);

	  	free(rxdata);
	  	free(root);
	  	free(json_str);
	  	pthread_mutex_unlock(&rec_mutex);
		return 0;
	};

	int observeridx = registerDeviceObserver(&recCb); 
	pthread_mutex_lock(&rec_mutex); 
	pthread_mutex_unlock(&rec_mutex);
	removeDeviceObserver(observeridx);
	printf("return http_rx_device_protocol\n");
	return 0;
}



static int event_handler(struct mg_event *event) {

	if (event->type == MG_REQUEST_BEGIN) {
	  
		if (strcmp(event->request_info->uri, "/switch") == 0) {
			return http_switch(event);
		} else if (strcmp(event->request_info->uri, "/rec_rx_data") == 0) {
			return http_rec_rx_data(event);
		}
		else if (strcmp(event->request_info->uri, "/rx_device_protocol") == 0) {
			return http_rx_device_protocol(event);
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
		"num_threads", "3",
		NULL
	};

	// Start the web server.
	webctx = mg_start(options, &event_handler, NULL);
}

void stopweb(void) {
	 mg_stop(webctx);
}
