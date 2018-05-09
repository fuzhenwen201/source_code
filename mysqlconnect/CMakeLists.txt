cmake_minimum_required(VERSION 2.8.4)
project(mysql)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(CLIENT_SOURCE_FILE mysqlconnect.cpp main.cpp)

add_executable(mysql ${CLIENT_SOURCE_FILE})

target_link_libraries(mysql mysqlcppconn)
