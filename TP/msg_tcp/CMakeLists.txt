# TP DEPT.
# Savin Chen
cmake_minimum_required(VERSION 2.8)
project(MsgServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11  -DKEEP_ALIVE_TEST -DDEBUG  -DLOCALHOSTIP ")



include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/new_tnode/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/include)



set(SOURCE_FILE 
	src/Public.cpp
	src/ConfigFileReader.cpp
	src/LogonModule.cpp
	src/HistDataModule.cpp
	src/BaseThread.cpp
	src/LRModule.cpp
	src/MemPoolMgr.cpp
	src/WokerModule.cpp
	src/marketdataagent.cpp
	src/Group_Spread.cpp
	src/ChangeNotice.cpp
	src/main.cpp
	)

set(OUTCLIENT
	src/Public.cpp
	src/tnodeconfig_v0.cpp
	src/marketdataagent.cpp
	src/ConfigFileReader.cpp
	src/msg_tcp_client.cpp
	) 




link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/new_tnode/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/lib)

add_executable(bin/MsgServer ${SOURCE_FILE})
add_executable(bin/msg_tcp_client ${OUTCLIENT})


target_link_libraries(bin/MsgServer curl tnode2 pthread rabbitmq ev xlogger 
glog sqlite3)
target_link_libraries(bin/msg_tcp_client curl tnode2  pthread rabbitmq ev xlogger glog)



