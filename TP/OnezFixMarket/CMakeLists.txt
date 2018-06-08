# TP DEPT.
# Savin Chen
cmake_minimum_required(VERSION 2.8)
project(OnezFixMarket)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11  -DKEEP_ALIVE_TEST -DDEBUG  -DLOCALHOSTIP ")



include_directories(${PROJECT_SOURCE_DIR}/fixinclude)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/new_tnode/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/include)



set(SOURCE_FILE 
	src/Public.cpp
	src/BaseThread.cpp
	src/RealtimeMarket.cpp
	src/RealtimeSender.cpp
	src/tnodeconfig_v0.cpp
	src/MemPoolMgr.cpp
	src/ConfigFileReader.cpp
	src/Group_Spread.cpp
	src/ChangeNotice.cpp
	src/main.cpp
	)



link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/new_tnode/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/lib)

add_executable(bin/OnezFixMarket ${SOURCE_FILE})


target_link_libraries(bin/OnezFixMarket quickfix tnode2 rabbitmq ev xlogger glog pthread sqlite3)


