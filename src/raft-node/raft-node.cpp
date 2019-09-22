#include "raft-node.h"

void RaftNode::Run() 
{
  std::thread t1(&RaftNode::RunP2PServer, this);
  t1.detach();

  while(true)
  {
    std::cout << "RAFTNODE: Debug printing in while loop\n" << std::endl;
    sleep(1);
  }
}

// TODO(ccdle12): First pass at heartbeat implementation.
void RaftNode::send_heart_beat(const std::string local_host, const int port)
{
    // TODO(ccdle12): heartbeat should be taken from a predefined variable.
    p2p_client_->Send(HEART_BEAT_MSG);
}

void RaftNode::RunP2PServer()
{
  p2p_server_->Listen();
}

void RaftNode::SendMsg(const uint8_t msg)
{
  p2p_client_->Send(msg);
}

const uint8_t* RaftNode::read_p2p_buffer() const
{
  return p2p_client_->get_buffer();
}

// ServerEventListener implementation. The p2p_server will callback to this 
// method when it receivese a message.
void RaftNode::MsgReceived(const int fd, const uint8_t *message) const
{
  std::cout << "DEBUG: CALLBACK WAS RECEIVED FROM THE P2P SERVER" << std::endl;
  if (HEART_BEAT_MSG == *message)
  {
      p2p_server_->SendResponse(fd, ACK);
  }
}
