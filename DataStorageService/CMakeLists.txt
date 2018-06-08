cmake_minimum_required(VERSION 2.8.4)
project(StorageData)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../rabbitmq/include /usr/local/protobuf/include)

set(SERVEER_SOURCE_FILE src/main.cc src/config.cc src/mysqlconnect.cpp src/datastorage.cc src/ME.Interface.pb.cc src/Ringbuffer.cpp)

link_directories(${PROJECT_SOURCE_DIR}/../rabbitmq/lib /usr/local/protobuf/lib)

add_executable(bin/StorageData ${SERVEER_SOURCE_FILE})

target_link_libraries(bin/StorageData tnode2 rabbitmq gflags xlogger glog pthread protobuf mysqlcppconn hiredis)
