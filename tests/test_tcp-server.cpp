#include <gtest/gtest.h>
#include "tcp-server/tcp-server.h"
#include <memory>

TEST(TestServerInit, OutputTest) {
    auto server = std::unique_ptr<Server>(new TCPServer(5040));
    server->Listen();
}

