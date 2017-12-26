#include "ControlPanel.h"
#include "HTTPServer.h"

#include <thread>

int main() {
  HTTPServer hs("8080", "cats.txt");
  hs.ServerLoop();
  //ControlPanel cp;
  //cp.InputLoop();
}
