# Exchange DEPT.
# Savin Chen
cmake_minimum_required(VERSION 2.8.4)
project(ChainOperateSrv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11  -DKEEP_ALIVE_TEST -DDEBUG  -DLOCALHOSTIP ")

include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/chain_inc)
#include_directories(${PROJECT_SOURCE_DIR}/../rabbitmq/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/db/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/blockchain/include/)  

set(SOURCE_FILE
	chain_src/uint128_t.cpp
	src/Public.cpp
	src/Main.cpp
	src/BlockConfirm.cpp
	src/ChainCreate.cpp
	src/DepositQuery.cpp
	src/ConfigFileReader.cpp
	src/TransferCoin.cpp
	chain_src/BlockChainOperator.cpp
	chain_src/EthJsonRPC.cpp
    chain_src/HttpClient.cpp
    chain_src/BitCoinJsonRPC.cpp)

link_directories(${PROJECT_SOURCE_DIR}/lib)
#link_directories(${PROJECT_SOURCE_DIR}/../rabbitmq/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/db/lib)
link_directories(/usr/local/lib/mysql)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/blockchain/lib)

add_executable(bin/ChainOperateSrv ${SOURCE_FILE})

target_link_libraries(bin/ChainOperateSrv tnode2 rabbitmq xlogger glog pthread mysqlop mysqlclient curl)
