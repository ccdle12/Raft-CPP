#include <gtest/gtest.h>
#include "raft-node/raft-node.h"
# include "constants.h"
#include <memory>
#include <thread>
#include <stdio.h>

TEST(TestRaftNode, SendSimpleMessage) {
    // Create Nodes and run them on different threads.
    RaftNode node = RaftNode(LOCAL_HOST, DEFAULT_PORT); 
    std::thread t1(&RaftNode::Run, &node);
    t1.detach();

    RaftNode node_2 = RaftNode(LOCAL_HOST, 5042); 
    std::thread t2(&RaftNode::Run, &node_2);
    t2.detach();

    // Send a message from node_2 to node_1.
    uint8_t msg = 0xB;
    node_2.SendMsg(msg);
    
    // Read the response message on node_2 from it's buffer. It should be
    // the same message sent.
    const uint8_t* response = node_2.read_p2p_buffer();
    printf("Returned message: %X\n", *response);

    ASSERT_EQ(*response, msg);
    ASSERT_EQ(*response, 11);
    ASSERT_EQ(*response, 0xB);
}
