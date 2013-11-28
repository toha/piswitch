#include "http.h"
#include "../rf/tx.h"
#include "../rf/rx.h"

struct mg_context *webctx;

int testsend(struct mg_event *event) {
	printf("testsend\n");
	protocol_t* p;
	p = (protocol_t*) malloc(sizeof *p);
	if (p != NULL) {
		p->type = PROTOCOL5;
		protocol5* p5;
		p5 = (protocol5*) malloc(sizeof *p5);
		if (p5 != NULL) {
			p5->network = 2716;
			p5->address = 2;
			p5->broadcast = 0;
			p5->state = 0;
			p5->dimmer = 0;
			p->p5 = *p5;
			tx_data_n_times(p, 3);

			free(p5);
		}

		free(p);
	}

	return 0;
}

static int event_handler(struct mg_event *event) {

  if (event->type == MG_REQUEST_BEGIN) {
    
		if (strcmp(event->request_info->uri, "/pola") == 0) {
				return testsend(event);
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

  // List of options. Last element must be NULL.
  //const char *options[] = {"listening_ports", "8080", NULL};

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
