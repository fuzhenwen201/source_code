cmake_minimum_required(VERSION 2.8)
project(main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILE src/mysqlop.cc include/mysqlop.h)

link_directories(${PROJECT_SOURCE_DIR}/lib)

#add_executable(main ${SOURCE_FILE})

#target_link_libraries(main glog gflags mysqlclient)

add_library(mysqlop SHARED src/mysqlop.cc)
