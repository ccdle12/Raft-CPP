# Enable use of CTest.
include(CTest)

# Set the variable for the test binary.
set(TEST_BINARY ${PROJECT_NAME}_test)

# Include the test files as an executable.
add_executable(${TEST_BINARY}
                test_tcp-client.cpp
                test_raft-node.cpp)

# Link the libraries to the test binary.
target_link_libraries(${TEST_BINARY}
                      tcp-server
                      tcp-client
                      raft-node
                      ${GTEST} gtest_main)

# Add the tests. 
add_test(
    NAME ${TEST_BINARY}
    COMMAND ${EXECUTABLE_OUTPUT_PATH}/${TEST_BINARY})
