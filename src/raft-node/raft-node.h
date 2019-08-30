#pragma once
#include <arpa/inet.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "tcp-server/tcp-server.h"
#include "tcp-client/tcp-client.h"

class RaftNode {
  public:
    RaftNode(std::string node_addr, unsigned int p2p_port) : p2p_port_{p2p_port}, node_addr_{node_addr} 
    {
      p2p_server_ = std::unique_ptr<Server>(new TCPServer(p2p_port_));
      p2p_client_ = std::unique_ptr<NetworkClient>(new TCPClient(node_addr_, p2p_port_, AF_INET));
    };

    void Run();
    void SendMsg(const uint8_t msg);
    const uint8_t* ReadP2PBuffer() const;

  private:
    unsigned int p2p_port_;
    std::string node_addr_;

    std::unique_ptr<Server> p2p_server_;
    std::unique_ptr<NetworkClient> p2p_client_;

    void RunP2PServer();
};
