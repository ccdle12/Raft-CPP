#include <gtest/gtest.h>
#include "raft-node/raft-node.h"
#include <memory>
#include <thread>

const std::string LOCAL_HOST="127.0.0.1";
const unsigned int LOCAL_PORT=5041;

TEST(TestRaftNodeInit, UnitTest) {
    RaftNode node = RaftNode(LOCAL_HOST, LOCAL_PORT); 
}

TEST(TestRaftNodeMsg, IntegrationTest) {
    // Create Nodes and run them on different threads.
    RaftNode node = RaftNode(LOCAL_HOST, LOCAL_PORT); 
    std::thread t1(&RaftNode::Run, &node);
    t1.detach();

    RaftNode node_2 = RaftNode(LOCAL_HOST, 5042); 
    std::thread t2(&RaftNode::Run, &node_2);
    t2.detach();

    // Assert that the ReadP2PBuffer is empty before expecting a response from 
    // the server.
    ASSERT_EQ(node_2.ReadP2PBuffer().length(), 0);

    // Send a message from node_2 to node_1.
    std::string msg = "Msg from client";
    node_2.SendMsg(msg);
    
    // Read the response from node_1 on node_2 from it's buffer, it should be
    // the same message sent.
    std::string recv_msg = node_2.ReadP2PBuffer();
    std::cout << "Returned msg from server: " << recv_msg << "\n" << std::endl;

    // Assert that the buffer, message received from the server is not empty.
    ASSERT_GT(recv_msg.length(), 0);
}
