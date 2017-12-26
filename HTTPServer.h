#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "mongoose.h"
#include <functional>
#include <iostream>
#include <experimental/filesystem>

#define PROGRAMMING_FILE "programming.csv"

namespace CDC {
  class HTTPServer {
  private:
    struct mg_mgr mgr;
    struct mg_connection *c;

  public:
    HTTPServer(std::string port);
    static void RequestHandler(struct mg_connection *c, int ev, void *p);
    void ServerLoop();
    ~HTTPServer();
  };
}

#endif
