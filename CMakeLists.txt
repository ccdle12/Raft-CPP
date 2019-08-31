project(c++_raft)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
message(STATUS "successfully set standard to c++11")

set(GTEST "gtest")
message(STATUS "successfully gtest variable")

include_directories(${PROJECT_SOURCE_DIR}/src)

# NOTE: ALL subdirectories must have a CMakeList.
add_subdirectory(${PROJECT_SOURCE_DIR}/src) # Add the src directory as subdirectory

enable_testing() # Must always be called from the top most CMakeLists.txt
add_subdirectory(tests) # Add the tests directory as subdirectory
add_subdirectory(${GTEST}) # Uses gtest as testing suite, adds the submodule gtest.


