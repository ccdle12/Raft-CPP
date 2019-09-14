#include "constants.h"
#include <gtest/gtest.h>
#include <memory>
#include "raft-node/raft-node.h"
#include <stdio.h>
#include <thread>

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

TEST(TestRaftNode, HeartBeat) {
    auto node = RaftNode(LOCAL_HOST, 5043);
    std::thread t1(&RaftNode::Run, &node);
    t1.detach();

    auto node_2 = RaftNode(LOCAL_HOST, 5044);
    std::thread t2(&RaftNode::Run, &node_2);
    t2.detach();

    // Send a heartbeat message from node_1 to node_2.
    node.send_heart_beat(LOCAL_HOST, 5044);

    // Read the response in the buffer. It should be a 'Ack': 0x02.
    const uint8_t* ack = node.read_p2p_buffer();
    printf("Ack: %X\n", *ack);

    ASSERT_EQ(*ack, 0x02);
}
