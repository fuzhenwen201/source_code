cmake_minimum_required(VERSION 2.8)
project(test_safe_queue)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDEBUG ")

set(SOURCE_FILE test_safe_queue.cc) 

add_executable(test_safe_queue ${SOURCE_FILE})

target_link_libraries(test_safe_queue pthread)
