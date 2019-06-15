#include <iostream>
#include "tcp-server/tcp-server.h"

int main() {
  TCPServer server = TCPServer(5041);
  std::cout << "Now running server listen" << std::endl;
  server.Listen();
  return 0;
}
