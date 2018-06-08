#ifndef _REDIS_H_
#define _REDIS_H_

#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <vector>
#include "log.h"
#include "const.h"
#include "redis/hiredis.h"
#include "redis/async.h"
#include "redis/libevent.h"

typedef void (*Fun)(redisAsyncContext *c, void *r, void *priv);
class CRedis
{
	public:

		CRedis();

		~CRedis();

		bool connect(std::string host, int port, int timeout, std::string strPwd, std::string strRdsins);

		bool asynconnect(std::string host, int port, int timeout, std::string strPwd, std::string strRdsins,
						struct event_base *base);


		bool asyncsubscribe(std::string key,Fun fn,void* param);
		bool asyncpsubscribe(std::string key, Fun fn, void* param);

		std::string get(std::string key);

		void del(std::string key);

		void set(std::string key, std::string value);

		void set(std::string key, std::string value, int sec);

		void hset(std::string key, std::string fild, std::string value);

		void setexpire(std::string key,int sec);

		void hset(std::string key,std::string fild, std::string value,int sec);

		std::string hgetall(std::string key);

		std::string hget(std::string key, std::string fild);

		void hdel(std::string key, std::string fild);

		int getkeyssize(std::string key);

		void getkeyfield(std::string key,std::vector<string>& fieldvactor);

	private:

		redisContext* _connect;
		redisReply* _reply;
		redisAsyncContext* _asynconnect;
		//pthread_mutex_t m_mutex;

};

#endif  //_REDIS_H_
