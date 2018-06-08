cmake_minimum_required(VERSION 2.8.4)
project(subscriptionservice)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../rabbitmq/include /usr/local/protobuf/include)

set(SERVEER_SOURCE_FILE server/main.cc server/SubscriptionServer.cc server/SubscriptionService.cc server/config.cc server/subscription.pb.cc)
set(CLIENT_SOURCE_FILE client/main.cc client/BondsProClient.cc client/guuid.cpp)

link_directories(${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/../rabbitmq/lib /usr/local/protobuf/lib)

add_executable(bin/client/client ${CLIENT_SOURCE_FILE})
add_executable(bin/server/SubscriAPI ${SERVEER_SOURCE_FILE})

target_link_libraries(bin/client/client quickfix)
target_link_libraries(bin/server/SubscriAPI quickfix tnode2 rabbitmq gflags xlogger glog pthread protobuf hiredis)
