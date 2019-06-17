#include <gtest/gtest.h>
#include "raft-node/raft-node.h"
#include <memory>
#include <thread>

const std::string LOCAL_HOST="127.0.0.1";
const unsigned int LOCAL_PORT=5040;

TEST(TestRaftNodeInit, OutputTest) {
    RaftNode node = RaftNode(LOCAL_HOST, LOCAL_PORT); 
}
