/*************************************************************************
    > File Name: mysqlconnect.cpp
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Fri 09 Mar 2018 09:38:02 AM CST
 ************************************************************************/

#include "mysqlconnect.h"

ConnectionPool* ConnectionPool::pool = nullptr;

//private
//构造函数
ConnectionPool::ConnectionPool(std::string name, std::string pwd, std::string nurl, int maxSize):
        username(name), password(pwd), url(nurl), poolSize(maxSize)
{
    //得到mysql驱动
    driver = get_driver_instance();
    //开始初始化大小一半
    initConnectPool(poolSize/2);
}

//析构函数
ConnectionPool::~ConnectionPool()
{
    destoryPool();
}

//得到连接池大小
int
ConnectionPool::getPoolSize()
{
    return conList.size();
}

//增加连接
void
ConnectionPool::addConn(int size)
{
    for(int i = 0; i < size; ++i)
    {
        //创建连接
        Connection *conn = driver->connect(url, username, password);
        std::shared_ptr<Connection> sp(conn,
                                       [](Connection *conn){
                                           delete conn;
                                       });
        conList.push_back(std::move(sp));
    }
}

//初始化连接池
void
ConnectionPool::initConnectPool(int initialSize)
{
    //加锁，增添一个连接
    std::lock_guard<std::mutex> locker(lock);
    addConn(initialSize);
}

//销毁一个连接
void
ConnectionPool::destoryOneConn()
{
    //智能指针加std::move转移一个连接的“所有权”，当出作用域时，自动调用关闭connect
    std::shared_ptr<Connection> &&sp = std::move(conList.front());
    sp->close();
    --poolSize;
}

//销毁整个连接池
void
ConnectionPool::destoryPool()
{
    for(auto &conn : conList)
    {
        //依次转移所有权，出作用域时，关闭连接，出作用域时智能指针自动释放
        std::shared_ptr<Connection> &&sp = std::move(conList.front());
        sp->close();
    }
}

//扩大连接池
void
ConnectionPool::expandPool(int size)
{
    std::lock_guard<std::mutex> locker(lock);
    addConn(size);
    poolSize += size;
}

//缩小连接池
void
ConnectionPool::reducePool(int size)
{
    std::lock_guard<std::mutex> locker(lock);
    //减小的大小不能超过存储的大小
    if(size > poolSize)
    {
        return;
    }
    for(int i = 0; i < size; i++)
    {
        //sp point new object, old object release
        destoryOneConn();
    }
    poolSize -= size;
}

//public
//得到连接池实例
ConnectionPool* ConnectionPool::getInstance(std::string name, std::string pwd, std::string nurl, int maxSize)
{
    if(pool == nullptr)
    {
        pool = new ConnectionPool(std::move(name), std::move(pwd), std::move(nurl), maxSize);
    }
    return pool;
}

//得到一个连接
std::shared_ptr<Connection> ConnectionPool::getConnect()
{
    std::lock_guard<std::mutex> locker(lock);
    std::shared_ptr<Connection> sp = conList.front();
    conList.pop_front();
    return sp;
}

//归还一个连接
void
ConnectionPool::retConnect(std::shared_ptr<Connection> &ret)
{
    std::lock_guard<std::mutex>locker(lock);
    conList.push_back(ret);
}