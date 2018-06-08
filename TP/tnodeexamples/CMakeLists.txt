cmake_minimum_required(VERSION 2.8)
project(tnodeexamples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g3")

include_directories(${PROJECT_SOURCE_DIR}/include)


set(SOURCE_FILES main.cpp tnodeconfig_v0.cc)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(tnodeexamples ${SOURCE_FILES})

target_link_libraries(tnodeexamples tnode amqp-cpp ev pthread)
