cmake_minimum_required(VERSION 2.8)
project(positionmanagement)
find_package(gflags REQUIRED)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 ")
#set(CMAKE_VERBOSE_MAKEFILE on)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDPOSITION -DMULTIPLE_THREAD -DKEEP_ALIVE_TEST -DDEBUG ")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDPOSITION -DMULTIPLE_THREAD -DKEEP_ALIVE_TEST -DDEBUG  -DREMOTE154")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDPOSITION -DMULTIPLE_THREAD -DKEEP_ALIVE_TEST -DDEBUG  -DLOCALHOSTIP")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDPOSITION -DKEEP_ALIVE_TEST -DDEBUG -DLOCALHOSTIP")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDPOSITION -DMULTIPLE_THREAD -DDEBUG")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDPOSITION -DMULTIPLE_THREAD ")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDPOSITION -DMULTIPLE_THREAD")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -pg -DDPOSITION -DMULTIPLE_THREAD")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDPOSITION -DDEBUG")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDEBUG ")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xflagger/include)
# tnode include
#include_directories(${PROJECT_SOURCE_DIR}/../tnodeexamples/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/db/include/)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/config/tnode/include)
#include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/market/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/include)

set(SOURCE_FILE main.cc 
	# xlogger
	#${PROJECT_SOURCE_DIR}/../xlogger/xlogger.cc
	# tnode
	${PROJECT_SOURCE_DIR}/../comm-lib/config/tnode/src/tnodeconfig_v0.cc
	# db
	#${PROJECT_SOURCE_DIR}/../db/src/mysqlop.cc
	#${PROJECT_SOURCE_DIR}/../db/src/redisop.cc
	#${PROJECT_SOURCE_DIR}/../xflagger/src/xconfig.cc
	#${PROJECT_SOURCE_DIR}/../xflagger/src/solution_config.cc
	#src/database_operator.cc
	src/define.cc
	src/xdatetime.cc
	src/position_list.cc
	src/position_subscribe.cc
	src/publish_exception.cc
	src/tnode_adapter.cc)

link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/market/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/db/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xflagger/lib)

add_executable(positionmanagement ${SOURCE_FILE})

target_link_libraries(positionmanagement tnode amqp-cpp ev xlogger solution_config xconfig mysqlop redisop gflags glog pthread mysqlclient hiredis uv uuid)
#target_link_libraries(positionmanagement uv tnode amqp-cpp ev glog atomic pthread)
