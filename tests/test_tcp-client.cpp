#include <arpa/inet.h>
#include <gtest/gtest.h>
#include "constants.h"
#include "tcp-client/tcp-client.h"

TEST(TCPClientTest, CorrectIPV) {
     // Tests that we can create a TCPClient using IPV4.
     TCPClient client = TCPClient(LOCAL_HOST, DEFAULT_PORT, AF_INET);
     ASSERT_EQ(client.IPV_, AF_INET);
}

TEST(TCPClientTest, NonExistentIPV) {
     // Tests that an error is thrown using a non-existent IP Version.
    ASSERT_THROW(
        TCPClient(LOCAL_HOST, DEFAULT_PORT, 14519251), std::runtime_error);
}
