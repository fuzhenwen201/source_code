cmake_minimum_required(VERSION 2.8)
project(mgsg)
find_package(gflags REQUIRED)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 ")
#set(CMAKE_VERBOSE_MAKEFILE on)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDSOLUTION -DMULTIPLE_THREAD -DKEEP_ALIVE_TEST -DDEBUG ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDSOLUTION -DMULTIPLE_THREAD -DKEEP_ALIVE_TEST -DDEBUG  -DLOCALHOSTIP")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DSIGNALENGINE -DMULTIPLE_THREAD -DKEEP_ALIVE_TEST -DDEBUG  -DLOCALHOSTIP") #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDSOLUTION -DMULTIPLE_THREAD -DKEEP_ALIVE_TEST -DDEBUG  -DREMOTE154") #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDSOLUTION -DMULTIPLE_THREAD -DKEEP_ALIVE_TEST ") #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -DDEBUG ")

#include_directories(${PROJECT_SOURCE_DIR}/http-parser)
include_directories(${PROJECT_SOURCE_DIR}/include)
#include_directories(${PROJECT_SOURCE_DIR}/../xflagger/include)
# tnode include
#include_directories(${PROJECT_SOURCE_DIR}/../tnodeexamples/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/config/tnode/include)

include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/market/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xflagger/include)

set(SOURCE_FILE main.cc 
	# xlogger
	#${PROJECT_SOURCE_DIR}/../xlogger/xlogger.cc
	# tnode-config
	${PROJECT_SOURCE_DIR}/../comm-lib/config/tnode/src/tnodeconfig_v0.cc
	# db
	#${PROJECT_SOURCE_DIR}/../db/src/mysqlop.cc
	#${PROJECT_SOURCE_DIR}/../db/src/redisop.cc
	#${PROJECT_SOURCE_DIR}/../xflagger/src/xconfig.cc
	#${PROJECT_SOURCE_DIR}/../xflagger/src/solution_config.cc
	#http-parser/http_parser.cc 
	src/define.cc
	#src/xdatetime.cc
	src/session_list.cc
	src/solution_subscribe.cc
	src/tnode_adapter.cc
	#src/http_request.cc
	src/connection.cc
	src/simplebuf.cc)


#link_directories(${PROJECT_SOURCE_DIR}/../tnodeexamples/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/market/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/lib)

link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xflagger/lib)
# link amqp-cpp
add_executable(mgsg ${SOURCE_FILE})

#target_link_libraries(SolutionGateway tnode marketagent xlogger uv amqp-cpp ev glog pthread mysqlclient)
#target_link_libraries(solutiongateway tnode marketagent xlogger solution_config xconfig mysqlop redisop hiredis uv amqp-cpp ev gflags glog pthread uuid mysqlclient)
target_link_libraries(mgsg tnode marketagent xlogger solution_config xconfig uv amqp-cpp ev gflags glog pthread uuid)

#target_link_libraries(SolutionGateway uv tnode amqp-cpp ev glog atomic pthread)
