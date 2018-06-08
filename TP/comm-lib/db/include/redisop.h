/*************************************************************************
	> File Name: redisop.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Mon 06 Mar 2017 07:43:34 PM CST
 ************************************************************************/

#ifndef DB_REDISOP_H_
#define DB_REDISOP_H_

#include<memory>	// for share_ptr
#include<exception>	// for exception
#include<mutex>		// for std::mutex
#include<string>
#include<vector>
#include<hiredis/hiredis.h>

#define DB_REDIS_LOGGER		"redisop"
#define DB_REDIS_LOGGER_DIR	"/home/leaker/tmp/logger/redis"
#define MAX_REDIS_OBJECT	4
#define REDIS_TIMEOUT_SEC	10 
#define REDIS_TIMEOUT_MICR	5000
namespace snetwork_xservice_db {
/*****Redis begin ****/	
class Redis {
	public:
		//Redis(const char* host = "127.0.0.1", int port = 6379, int db = 0, const char* pwd = "hugo123456");
		Redis();
		~Redis();

		Redis(const Redis& r) = delete;
		Redis operator=(const Redis& r) = delete;
		Redis(Redis&& r) = delete;
		Redis& operator=(Redis&& r) = delete;

	public:
		bool Connection(void);
		bool SelectDB(int db);
		bool IsConnect(void); 
		std::string HGet(const char* key, const char* field);
		int HSet(const char* key, const char* field, const char* value);
		/*for binary set*/
		int HSet(const char* key, const char* field, const char* value, size_t valueLen);
		int Del(const char* key);
		int ExistsKey(const char* key);
	private:
		int m_port;
		int m_db;
		char m_host[16];
		char m_pwd[64+1];
		redisContext* m_redisContext;
};
/*****Redis end ****/	
};
#endif

