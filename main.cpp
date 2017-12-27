#include "ControlPanel.h"
#include "HTTPServer.h"

#include <thread>

int main() {
  /*
  auto serverFn = [] () -> void {
    CDC::HTTPServer hs("8080");
    hs.ServerLoop();
  };

  std::thread httpServerThread(serverFn);
  httpServerThread.detach();
  */
  CDC::FileOps::check_hmac("/bin/ls", "a");
  //CDC::ControlPanel cp;
  //cp.InputLoop();
}
