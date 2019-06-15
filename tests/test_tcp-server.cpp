#include <gtest/gtest.h>
#include "tcp-server/tcp-server.h"
#include <memory>
#include <thread>


TEST(TestServerInit, OutputTest) {
    // Run the TCPServer on a background thread.
    TCPServer server = TCPServer(5040);
    std::thread t1(&TCPServer::Listen, &server);
    t1.detach();

    // Sleep to give some time for the thread to run.
    usleep(500);

    // Force close the server.
    server.Close();
}
