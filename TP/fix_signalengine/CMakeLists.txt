# TP DEPT.
# Savin Chen
cmake_minimum_required(VERSION 2.8)
project(fix_se)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11  -DKEEP_ALIVE_TEST -DDEBUG  -DLOCALHOSTIP ")



include_directories(${PROJECT_SOURCE_DIR}/fixinclude)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/include)



set(SOURCE_FILE 
	src/Public.cpp
	src/BaseThread.cpp
	src/RealtimeMarket.cpp
	src/RealtimeSender.cpp
	src/tnodeconfig_v0.cpp
	src/MemPoolMgr.cpp
	src/ConfigFileReader.cpp
	src/main.cpp
	)



link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/lib)

add_executable(bin/fix_se ${SOURCE_FILE})


target_link_libraries(bin/fix_se quickfix tnode amqp-cpp ev xlogger glog pthread)


