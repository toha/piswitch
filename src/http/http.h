#include <stdio.h>
#include <string.h>
#include "../lib/mongoose/mongoose.h"
// request handler
int begin_request_handler(struct mg_connection *conn);

// start webserver
int startweb(void);
