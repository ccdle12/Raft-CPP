#pragma once
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
      p2p_client_ = std::unique_ptr<Client>(new TCPClient(node_addr_, p2p_port_));
    };

    void Run();
    void SendMsg(const std::string& msg);
    const std::string& ReadP2PBuffer() const;

  private:
    unsigned int p2p_port_;
    std::string node_addr_;

    std::unique_ptr<Server> p2p_server_;
    std::unique_ptr<Client> p2p_client_;

    // Internal Methods.
    void RunP2PServer();
};
