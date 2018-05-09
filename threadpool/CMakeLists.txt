cmake_minimum_required(VERSION 2.8.4)
project(threadpool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.cc
    threadpool.h)

add_executable(threadpool ${SOURCE_FILES})
target_link_libraries(threadpool pthread)