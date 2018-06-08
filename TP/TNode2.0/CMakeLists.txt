cmake_minimum_required(VERSION 2.8)
project(new)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g3 -Wall")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/include)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(SOURCE_FILES 
	src/tnode.cpp
	src/consumer.cpp 
	)


set(TNOE2_PUBLISHER src/tnode2_publisher.cpp) 
set(TNOE2_CONSUMER src/tnode2_consumer.cpp)

# 生成动态库目标
add_library(tnode2 SHARED ${SOURCE_FILES})
# 生成静态库目标
add_library(tnode_static STATIC ${SOURCE_FILES})

# 指定静态库的输出名称
set_target_properties(tnode_static PROPERTIES OUTPUT_NAME "tnode2")
# 使动态库和静态库同时存在
set_target_properties(tnode2 PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(tnode_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 指定动态库版本
# VERSION 动态库版本
# SOVERSION API版本
set_target_properties(tnode2 PROPERTIES VERSION 0.1 SOVERSION 0.1)

link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/lib)




add_executable(tnode2_publisher  ${TNOE2_PUBLISHER}) 
add_executable(tnode2_consumer  ${TNOE2_CONSUMER}) 
target_link_libraries(tnode2_publisher xlogger tnode2 glog rabbitmq pthread) 
target_link_libraries(tnode2_consumer xlogger tnode2 glog rabbitmq pthread) 


