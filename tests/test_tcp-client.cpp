#include <arpa/inet.h>
#include <exception>
#include <gtest/gtest.h>
#include <unistd.h>
#include "constants.h"
#include "tcp-client/tcp-client.h"

TEST(TCPClientTest, CorrectIPV) {
     TCPClient client = TCPClient(LOCAL_HOST, DEFAULT_PORT, AF_INET);
     ASSERT_EQ(client.IPV_, AF_INET);
}

TEST(TCPClientTest, NonExistentIPV) {
    ASSERT_THROW(
        TCPClient(LOCAL_HOST, DEFAULT_PORT, 14519251), std::runtime_error);
}
