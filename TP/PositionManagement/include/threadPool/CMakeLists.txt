cmake_minimum_required(VERSION 2.8)
project(test_thread_pool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDEBUG ")

set(SOURCE_FILE test_thread.cc) 
include_directories(${PROJECT_SOURCE_DIR}/../)

add_executable(test_thread_pool ${SOURCE_FILE})

target_link_libraries(test_thread_pool pthread)
