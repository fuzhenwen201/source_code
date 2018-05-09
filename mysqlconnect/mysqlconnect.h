/*************************************************************************
    > File Name: mysqlconnect.h
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Fri 09 Mar 2018 05:12:17 PM CST
 ************************************************************************/

#ifndef _MYSQL_CONNECTION_
#define _MYSQL_CONNECTION_

#include<iostream>
using namespace std;
//c++
#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <functional>

//mysql driver
#include <mysql_driver.h>
#include <mysql_connection.h>

//mysql execute
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <exception>

//thread mutex
#include <mutex>
using namespace sql;
using delFunc = std::function<void(Connection*)>;

class ConnectionPool
{
public:
    //获取数据库连接池对象 static单例模式
    static ConnectionPool* getInstance(std::string name, std::string pwd, std::string nurl, int maxSize);
    //得到一条连接
    auto getConnect()->std::shared_ptr<Connection>;
    //归还一条连接
    auto retConnect(std::shared_ptr<Connection> &ret)->void;
    ~ConnectionPool();

private:
    ConnectionPool(std::string name, std::string pwd, std::string nurl, int maxSize);
    //初始化连接池
    auto initConnectPool(int initialSize)->void;
    //毁坏连接池
    auto destoryPool()->void;
    //destory one connection
    auto destoryOneConn()->void;
    //扩大数据库连接池
    auto expandPool(int size)->void;
    //缩小数据库连接池
    auto reducePool(int size)->void;
    //add conn
    auto addConn(int size)->void;
public:
    //get size
    auto getPoolSize()->int;
private:
    std::string username; //帐号
    std::string password; //密码
    std::string url;      //连接url
    int poolSize;       //pool size
    //存放所有连接
    std::list<std::shared_ptr<Connection>> conList;
    static ConnectionPool *pool;//连接池对象
    std::mutex lock;//锁
    Driver *driver;//mysql driver
};
#endif
