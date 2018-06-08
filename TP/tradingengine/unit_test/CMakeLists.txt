cmake_minimum_required(VERSION 2.8)
project(unit_test)

find_package(gflags REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -g3")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../../comm-lib/tnode/include ${PROJECT_SOURCE_DIR}/../../comm-lib/xlogger/include  ${PROJECT_SOURCE_DIR}/../include  ${PROJECT_SOURCE_DIR}/../../googletest/googletest/include )

set(SOURCE_FILES te_unit_test.cpp ../src/tradingengine.cc ../src/tnodeconfig_v0.cc)
link_directories(${PROJECT_SOURCE_DIR}/../../comm-lib/tnode/lib ${PROJECT_SOURCE_DIR}/../../comm-lib/xlogger/lib ${PROJECT_SOURCE_DIR}/../../googletest/googletest)

add_executable(unit_test ${SOURCE_FILES})
target_link_libraries(unit_test  tnode  pthread amqp-cpp ev xlogger glog  gflags gtest gtest_main )
