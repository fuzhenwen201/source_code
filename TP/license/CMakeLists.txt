cmake_minimum_required(VERSION 2.8.4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES_so src/hexutil.cpp  src/Teaencrypt.cpp)
set(SOURCE_FILES main.cpp src/hexutil.cpp  src/Teaencrypt.cpp)

add_library(security SHARED ${SOURCE_FILES_so})
add_executable(security.exe ${SOURCE_FILES})
