#include "HTTPServer.h"

CDC::HTTPServer::HTTPServer(const char *port) {
  mg_mgr_init(&mgr, NULL);

  c = mg_bind(&mgr, port, HTTPServer::RequestHandler);
  if (c == NULL) {
    std::cerr << "Unable to bind to port " << port << std::endl;
    exit(-1);
  }
  mg_set_protocol_http_websocket(c);
}

CDC::HTTPServer::~HTTPServer() {
  mg_mgr_free(&mgr);
}

void CDC::HTTPServer::RequestHandler(struct mg_connection *c, int ev, void *p) {
  struct http_message *hm = (struct http_message*)p;
  if (ev == MG_EV_HTTP_REQUEST) {
    if (mg_vcmp(&hm->uri, "/") == 0) {
      if (std::experimental::filesystem::exists(PROGRAMMING_FILE)) {
        mg_http_serve_file(c, hm, PROGRAMMING_FILE, mg_mk_str("text/plain"), mg_mk_str(""));
      }
      else {
        mg_send_head(c, 500, strlen("Programming file not found!"), "Content-Type: text/plain");
        mg_printf(c, "Programming file not found!");
      }
    }
    else {
      //404 not found
      mg_send_head(c, 404, strlen("Invalid URL - 404 not found"), "Content-Type: text/plain");
      mg_printf(c, "Invalid URL - 404 not found");
    }
    c->flags |= MG_F_SEND_AND_CLOSE;
  }
}

void CDC::HTTPServer::ServerLoop() {
  for(;;) {
    mg_mgr_poll(&mgr, 1000);
  }
}
