#pragma once
#include <arpa/inet.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "tcp-client/tcp-client.h"
#include "tcp-server/tcp-server.h"
#include "server_event_listener.h"

static const uint8_t HEART_BEAT_MSG = 0x01;
static const uint8_t ACK = 0x02;

class RaftNode : public ServerEventListener {
  public:
    RaftNode(std::string node_addr, unsigned int p2p_port) : p2p_port_{p2p_port}, node_addr_{node_addr} 
    {
      p2p_server_ = std::unique_ptr<NetworkServer>(new TCPServer(p2p_port_, AF_INET, *this));
      p2p_client_ = std::unique_ptr<NetworkClient>(new TCPClient(node_addr_, p2p_port_, AF_INET));
    };

    void Run();
    void SendMsg(const uint8_t msg);
    const uint8_t* read_p2p_buffer() const;

    // HeartBeat.
    void send_heart_beat(const std::string local_host, const int port);

    // Event Listener Implementation.
    void MsgReceived(const int index, const uint8_t *message) const override;

  private:
    unsigned int p2p_port_;
    std::string node_addr_;

    std::unique_ptr<NetworkServer> p2p_server_;
    std::unique_ptr<NetworkClient> p2p_client_;

    void RunP2PServer();
};
