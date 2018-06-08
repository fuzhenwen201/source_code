cmake_minimum_required(VERSION 2.8)
project(main)

find_package(gflags REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(LIB_SOURCE_FILE 
	src/rxconfig.cc
	) 

set(SOURCE_FILE 
	src/main.cc
	src/rxconfig.cc
	)


add_executable(main ${SOURCE_FILE})


set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(rxconfig_static STATIC ${LIB_SOURCE_FILE})
add_library(rxconfig SHARED ${LIB_SOURCE_FILE})
set_target_properties(rxconfig_static PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}"
	)

SET_TARGET_PROPERTIES(rxconfig_static PROPERTIES OUTPUT_NAME "rxconfig")
GET_TARGET_PROPERTY(output_name rxconfig_static OUTPUT_NAME)
MESSAGE("This is the rxconfig_static output_name " ${output_name})

SET_TARGET_PROPERTIES(rxconfig PROPERTIES CLEAN_DIRECT_OUTPUT ON)
SET_TARGET_PROPERTIES(rxconfig_static PROPERTIES CLEAN_DIRECT_OUTPUT ON)

SET_TARGET_PROPERTIES(rxconfig PROPERTIES VERSION 1.0)

TARGET_LINK_LIBRARIES(rxconfig_static
	gflags
	mysqlclient)

TARGET_LINK_LIBRARIES(main
	gflags
	)


