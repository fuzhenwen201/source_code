# TP DEPT.
# Savin Chen
cmake_minimum_required(VERSION 2.8)
project(SGDistributor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11  -DKEEP_ALIVE_TEST -DDEBUG  -DLOCALHOSTIP ")



include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(/usr/local/libevent/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/new_tnode/include)

set(SOURCE_FILE 
	src/ConfigFileReader.cpp
	src/Public.cpp
	src/BaseThread.cpp
	src/InService.cpp
	src/LRModule.cpp
	src/MemPoolMgr.cpp
	src/WokerModule.cpp
	src/WorkeDataListMgr.cpp
	src/URLMap
	src/ChangeNotice.cpp
	src/main.cpp
	)



link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/new_tnode/lib)

add_executable(bin/SGDistributor ${SOURCE_FILE})

target_link_libraries(bin/SGDistributor tnode2 rabbitmq event pthread xlogger glog sqlite3)


