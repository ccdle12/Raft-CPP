#include <gtest/gtest.h>
#include "tcp-server/tcp-server.h"
#include "tcp-client/tcp-client.h"
#include <memory>
#include <thread>

const std::string LOCAL_HOST="127.0.0.1";
const unsigned int LOCAL_PORT=5040;

TEST(TestServerInit, OutputTest) {
    // Run the TCPServer on a background thread.
    // TCPServer server = TCPServer(5040);
    TCPServer server = TCPServer(LOCAL_PORT);
    std::thread t1(&TCPServer::Listen, &server);
    t1.detach();

    // Sleep to give some time for the thread to run.
    usleep(1000);

    // Force close the server.
    server.Close();
}

TEST(TestClientInit, OutputTest) {
  TCPClient client = TCPClient(LOCAL_HOST, LOCAL_PORT);
}

TEST(TestClientServer, OutputTest) {
    // Run the TCPServer on a background thread.
    TCPServer server = TCPServer(LOCAL_PORT);
    std::thread t1(&TCPServer::Listen, &server);
    t1.detach();

    // Sleep to give some time for the thread to run.
    usleep(1000);

    TCPClient client_1 = TCPClient(LOCAL_HOST, LOCAL_PORT);
    std::thread t2(&TCPClient::Send, &client_1);
    t2.detach();

    TCPClient client_2 = TCPClient(LOCAL_HOST, LOCAL_PORT);
    std::thread t3(&TCPClient::Send, &client_2);
    t3.join();
}
