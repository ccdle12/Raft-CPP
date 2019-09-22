#include "constants.h"
#include <gtest/gtest.h>
#include <memory>
#include "raft-node/raft-node.h"
#include <stdio.h>
#include <thread>

auto node_1 = RaftNode(LOCAL_HOST, DEFAULT_PORT);
auto node_2 = RaftNode(LOCAL_HOST, 5042);

TEST(TestRaftNode, HeartBeat) {
    std::thread t1(&RaftNode::Run, &node_1);
    if (t1.joinable())
        t1.detach();

    std::thread t2(&RaftNode::Run, &node_2);
    if (t2.joinable())
        t2.detach();

    // Send a heartbeat message from node_1 to node_2.
    node_1.send_heart_beat(LOCAL_HOST, 5042);

    // Read the response in the buffer. It should be a 'Ack': 0x02.
    const uint8_t* ack = node_1.read_p2p_buffer();
    printf("Ack: %X\n", *ack);

    ASSERT_EQ(*ack, 0x02);
}
