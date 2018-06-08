
cmake_minimum_required(VERSION 2.8)
project(tcpmodel)

find_package(gflags REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/include)

set(SOURCE_FILE	src/main.cpp include/public.h src/monitorbridge.cpp include/monitorbridge.h src/tnodeconfig_v0.cc)

link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode/lib)

add_executable(monitorbridge ${SOURCE_FILE})

target_link_libraries(monitorbridge tnode pthread amqp-cpp ev xlogger glog gflags)
