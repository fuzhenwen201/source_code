cmake_minimum_required(VERSION 2.8)
project(glogtest)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 ")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILE src/main.cc src/selfconfig.cc src/guuid.cpp)

link_directories(${PROJECT_SOURCE_DIR}/lib)
add_executable(glogtest ${SOURCE_FILE})

target_link_libraries(glogtest glog gflags pthread)

