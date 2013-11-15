#include "http.h"


struct mg_context *webctx;

// This function will be called by mongoose on every new request.
int begin_request_handler(struct mg_connection *conn) {
  char content[100];

  // Prepare the message we're going to send
  int content_length = snprintf(content, sizeof(content),
                                "PiSwitch Yeah!");

  // Send HTTP reply to the client
  mg_printf(conn,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "\r\n"
            "%s",
            content_length, content);

  // Returning non-zero tells mongoose that our function has replied to
  // the client , and mongoose should not send client any more data.
  return 1;
}

void startweb(void) {
  
  struct mg_callbacks callbacks;

  // List of options. Last element must be NULL.
  const char *options[] = {"listening_ports", "1338", NULL};

  // Prepare callbacks structure. We have only one callback, the rest are NULL.
  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.begin_request = begin_request_handler;

  // Start the web server.
  webctx = mg_start(&callbacks, NULL, options);
}

void stopweb(void) {
	 mg_stop(webctx);
}
