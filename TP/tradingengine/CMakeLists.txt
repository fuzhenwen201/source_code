cmake_minimum_required(VERSION 2.8)
project(tradingengine)

find_package(gflags REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../comm-lib/tnode/include ${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/include  ${PROJECT_SOURCE_DIR}/../comm-lib/market/include)

set(SOURCE_FILES src/main.cc src/tradingengine.cc include/queuemanager.h src/tnodeconfig_v0.cc include/minheap.h src/TEconfig.cc src/groupspread.cc)

link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode/lib ${PROJECT_SOURCE_DIR}/../comm-lib/market/lib ${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/lib) 

add_executable(tradingengine ${SOURCE_FILES})

target_link_libraries(tradingengine tnode pthread amqp-cpp ev xlogger glog gflags uuid sqlite3)
