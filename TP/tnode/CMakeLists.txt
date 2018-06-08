cmake_minimum_required(VERSION 2.8)
project(new)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g3 -Wall")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(SOURCE_FILES src/BaseThread.cpp src/ReadDataProcessor.cc src/tnoderabbitchannel.cc src/tnoderabbit.cc include/snetwork_typedef.h include/tnodeconsumer.h include/tnodeerrno.h include/tnoderabbitchannel.h include/tnodehandle.h include/tnode.h include/tnodeconfig.h include/tnoderabbit.h include/tnoderabbitchannel.h src/tnode.cc src/tnoderabbitmanager.cc include/tnoderabbitmanager.h src/tnodeexception.cc include/tnodeexception.h src/tnodelog.cc include/tnodelog.h include/tnodechannel.h src/tnodechannel.cc )

set(UINIT_TEST_FILES unittest/unittest.cc unittest/unittest.h src/tnodeconfig_v0.cc include/tnodeconfig_v0.h src/main.cc)
# 生成动态库目标
add_library(tnode SHARED ${SOURCE_FILES})
# 生成静态库目标
add_library(tnode_static STATIC ${SOURCE_FILES})

# 指定静态库的输出名称
set_target_properties(tnode_static PROPERTIES OUTPUT_NAME "tnode")
# 使动态库和静态库同时存在
set_target_properties(tnode PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(tnode_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 指定动态库版本
# VERSION 动态库版本
# SOVERSION API版本
set_target_properties(tnode PROPERTIES VERSION 0.3 SOVERSION 1)

#link_directories("/home/alex/ClionProjects/TNode-0.3/lib")
link_directories(${PROJECT_SOURCE_DIR}/lib)


add_executable(TNode-0.3  ${UINIT_TEST_FILES})
target_link_libraries(TNode-0.3 tnode amqp-cpp ev pthread)


