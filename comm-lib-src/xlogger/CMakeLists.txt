cmake_minimum_required(VERSION 2.8)
project(xlogger)

#find_package(glog)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 ")

set(SOURCE_FILES main.cc xlogger.cc)

add_library(xlogger SHARED ${SOURCE_FILES})

set_target_properties(xlogger PROPERTIES CLEAN_DIRECT_OUTPUT 1)

add_executable(xlogger_test ${SOURCE_FILES})

target_link_libraries(xlogger_test glog)


