cmake_minimum_required(VERSION 2.8)
project(main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=c11")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../xlogger)
include_directories(${PROJECT_SOURCE_DIR}/../xlogger/include)

set(LIB_SOURCE_FILE_MYSQLOP 
	src/mysqlop.cc) 

set(LIB_SOURCE_FILE_REDISOP 
	src/redisop.cc
	src/redis_command_table.c) 

set(SOURCE_FILE 
	src/main.cc
	src/mysqlop.cc 
	src/redisop.cc
	src/redis_command_table.c) 

link_directories(${PROJECT_SOURCE_DIR}/../xlogger)
link_directories(${PROJECT_SOURCE_DIR}/../xlogger/lib)

add_executable(main ${SOURCE_FILE})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(mysqlop_static STATIC ${LIB_SOURCE_FILE_MYSQLOP})
add_library(mysqlop SHARED ${LIB_SOURCE_FILE_MYSQLOP})
set_target_properties(mysqlop_static PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}"
	)

SET_TARGET_PROPERTIES(mysqlop_static PROPERTIES OUTPUT_NAME "mysqlop")
GET_TARGET_PROPERTY(output_name mysqlop_static OUTPUT_NAME)
MESSAGE("This is the mysqlop_static output_name " ${output_name})

SET_TARGET_PROPERTIES(mysqlop PROPERTIES CLEAN_DIRECT_OUTPUT ON)
SET_TARGET_PROPERTIES(mysqlop_static PROPERTIES CLEAN_DIRECT_OUTPUT ON)

SET_TARGET_PROPERTIES(mysqlop PROPERTIES VERSION 1.0)

add_library(redisop_static STATIC ${LIB_SOURCE_FILE_REDISOP})
add_library(redisop SHARED ${LIB_SOURCE_FILE_REDISOP})
set_target_properties(redisop_static PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}"
	)

SET_TARGET_PROPERTIES(redisop_static PROPERTIES OUTPUT_NAME "redisop")
GET_TARGET_PROPERTY(output_name redisop_static OUTPUT_NAME)
MESSAGE("This is the redisop_static output_name " ${output_name})

SET_TARGET_PROPERTIES(redisop PROPERTIES CLEAN_DIRECT_OUTPUT ON)
SET_TARGET_PROPERTIES(redisop_static PROPERTIES CLEAN_DIRECT_OUTPUT ON)

SET_TARGET_PROPERTIES(redisop PROPERTIES VERSION 1.0)

TARGET_LINK_LIBRARIES(mysqlop_static
	mysqlclient)

TARGET_LINK_LIBRARIES(redisop_static
	xlogger
	glog
	hiredis)

TARGET_LINK_LIBRARIES(main
	xlogger
	glog
	mysqlclient
	hiredis)

