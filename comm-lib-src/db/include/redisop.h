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
#include<string.h>
#include<hiredis/hiredis.h>
#include"redis_command_table.h"

#define REDIS_TIMEOUT_SEC	10 
#define REDIS_TIMEOUT_MICR	200
#define REDIS_RECONNECT		10
/*****Redis begin ****/	
void FreeReplyObject(redisReply* p);
class CRedisLock;

class CRedis {
	public:
		CRedis(const char* host, int port = 6379, int db = 0, const char* pwd = nullptr, int componentID = 0, int retryCount=3, int retryDelay=200);
		CRedis();
		~CRedis();

		CRedis(const CRedis& r) = delete;
		CRedis& operator=(const CRedis& r) = delete;
		CRedis(CRedis&& r) = delete;
		CRedis& operator=(CRedis&& r) = delete;

	public:
		//bool Connection(void);
		bool SelectDB(int db);
		bool IsConnect(void); 
		std::string HGet(const char* key, const char* field);
		int HSet(const char* key, const char* field, const char* value);
		/*for binary set*/
		int HSet(const char* key, const char* field, const char* value, size_t valueLen);
		int Del(const char* key);
		int ExistsKey(const char* key);

		std::shared_ptr<redisReply> Excute(enum RedisCom comIndex, int arc, const char** arv);
		std::shared_ptr<redisReply> Excute(const char* format, ...);

	private:
		bool Connection(void);
		long GetUniqueID(void) const;

	public:
		/* redis lock */
		bool Lock(CRedisLock& lk);
		bool UnLock(CRedisLock& lk);

	private:
		static int m_sIndex;
		static const char* m_unLockCmd;
		int m_port;
		int m_db;
		char m_host[16];
		char m_pwd[64+1];
		int m_componentID;
		int m_index;
		int m_retryCount;
		int m_retryDelay;
		int m_defaultLockTime;
		redisContext* m_redisContext;
};

class CRedisLock {
	public:
		CRedisLock(const char* key=nullptr, const char* val=nullptr, int validityTime=10000);
		~CRedisLock();

	private:
		CRedisLock(const CRedisLock& r) = delete;
		CRedisLock& operator=(const CRedisLock& r) = delete;

		CRedisLock(CRedisLock&& r) = delete;
		CRedisLock& operator=(CRedisLock&& r) = delete;

	public:
		void ValidityTime(int validityTime) {
			m_validityTime = validityTime;
		}
		int ValidityTime(void) {
			return m_validityTime;
		}

		void Key(const char* key) {
			if (m_key != nullptr && strcmp(m_key, key) != 0) {
				free(m_key);
				m_key = nullptr;
			}

			m_key = strdup(key);
		}
		const char* Key(void) const {
			return m_key;
		}

		void Value(const char* value) {
			if (m_value != nullptr && strcmp(m_value, value) != 0) {
				free(m_value);
				m_value = nullptr;
			}

			m_value = strdup(value);
		}
		const char* Value(void) const {
			return m_value;
		}

	private:
		char* m_key;
		char* m_value;
		long m_validityTime;
};
/*****Redis end ****/	
#endif

