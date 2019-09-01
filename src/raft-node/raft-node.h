#pragma once
#include <arpa/inet.h>
#include <iostream>
#include <memory>
#include <string>
#include "tcp-client/tcp-client.h"
#include "tcp-server/tcp-server.h"
#include <thread>

class RaftNode {
  public:
    RaftNode(std::string node_addr, unsigned int p2p_port) : p2p_port_{p2p_port}, node_addr_{node_addr} 
    {
      p2p_server_ = std::unique_ptr<NetworkServer>(new TCPServer(p2p_port_, AF_INET));
      p2p_client_ = std::unique_ptr<NetworkClient>(new TCPClient(node_addr_, p2p_port_, AF_INET));
    };

    void Run();
    void SendMsg(const uint8_t msg);
    const uint8_t* read_p2p_buffer() const;

  private:
    unsigned int p2p_port_;
    std::string node_addr_;

    std::unique_ptr<NetworkServer> p2p_server_;
    std::unique_ptr<NetworkClient> p2p_client_;

    void RunP2PServer();
};
