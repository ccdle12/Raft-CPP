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

void RaftNode::RunP2PServer()
{
  p2p_server_->Listen();
}

void RaftNode::SendMsg(const std::string& msg)
{
  p2p_client_->Send(msg);
}

const std::string& RaftNode::ReadP2PBuffer() const
{
  return p2p_client_->GetBuffer();
}