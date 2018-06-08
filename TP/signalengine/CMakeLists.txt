cmake_minimum_required(VERSION 2.8)
project(signalengine)

find_package(gflags REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -g3")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../comm-lib/tnode/include ${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/include)


set(SOURCE_FILES src/main.cc src/marketdata.cc src/tnodeconfig_v0.cc src/xconfig.cc src/SEconfig.cc)

set(AGENT_SOURCE_FILES src/marketdataagent.cc)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 生成动态库目标
add_library(marketagent SHARED ${AGENT_SOURCE_FILES})
# 生成静态库目标
add_library(marketagent_static STATIC ${AGENT_SOURCE_FILES})

# 指定静态库的输出名称
set_target_properties(marketagent_static PROPERTIES OUTPUT_NAME "marketagent")
# 使动态库和静态库同时存在
set_target_properties(marketagent PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(marketagent_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 指定动态库版本
# VERSION 动态库版本
# SOVERSION API版本
set_target_properties(marketagent PROPERTIES VERSION 0.1 SOVERSION 1)

link_directories(${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/../comm-lib/tnode/lib ${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/lib )

add_executable(signalengine ${SOURCE_FILES})
target_link_libraries(signalengine curl tnode marketagent pthread amqp-cpp ev xlogger glog  gflags)
