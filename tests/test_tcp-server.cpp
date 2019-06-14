#include <gtest/gtest.h>
#include "tcp-server/tcp-server.h"

TEST(TestServerInit, OutputTest) {
  TCPServer server = TCPServer(5040);
  server.Listen();
}

