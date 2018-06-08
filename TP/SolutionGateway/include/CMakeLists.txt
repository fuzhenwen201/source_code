cmake_minimum_required(VERSION 2.8)
project(test_safe_list)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDEBUG ")

set(SOURCE_FILE test_safe_list.cc) 

add_executable(test_safe_list ${SOURCE_FILE})
