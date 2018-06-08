cmake_minimum_required(VERSION 2.8)
project(BigNumber)

set(BigNumber_VERSION_MAJOR 1)
set(BigNumber_VERSION_MINOR 0)

set(c++version -std=c++11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -pedantic -Wno-unused-function ${c++version}")

set(CMAKE_BUILD_TYPE Release)

set(SOURCE_FILES  main.cc uint128_t.cpp)

add_executable(BigNumberRun ${SOURCE_FILES})
