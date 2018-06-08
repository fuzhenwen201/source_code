/*************************************************************************
	> File Name: redisop.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Mon 06 Mar 2017 07:42:04 PM CST
 ************************************************************************/

#include<unistd.h>
#include<cstdlib>
#include<cstring>
#include"redisop.h"
#include"xlogger.h"
#if defined(DSOLUTION)
#include"solution_config.h"
#elif defined(DPOSITION)
#include"solution_config.h"
#endif

using namespace snetwork_xservice_xflagger;
namespace snetwork_xservice_db {
#if 0
Redis::Redis(const char* host, int port, int db, const char* pwd) {
	memset(m_host, 0, sizeof(m_host));
	strncpy(m_host, host, 15);
	m_port = port;
	m_db = db;
	memset(m_pwd, 0, sizeof(m_pwd));
	if (pwd != nullptr) {
		strncpy(m_pwd, pwd, 64);
	}
	m_redisContext = nullptr;
	Connection();
}
#endif

Redis::Redis() {
	memset(m_host, 0, sizeof(m_host));
	memset(m_pwd, 0, sizeof(m_pwd));
#if defined(DSOLUTION)
	SXConfig* sxconfig = dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
	strncpy(m_host, sxconfig->RedisHost().c_str(), 15);
	strncpy(m_pwd, sxconfig->RedisAuth().c_str(), 64);
	m_port = sxconfig->RedisPort();
	m_db = sxconfig->RedisDB();
#elif defined(DPOSITION)
	SXConfig* sxconfig = dynamic_cast<SXConfig*>(SXFlagger::GetInstance()->GetConfig());
	strncpy(m_host, sxconfig->RedisHost().c_str(), 15);
	strncpy(m_pwd, sxconfig->RedisAuth().c_str(), 64);
	m_port = sxconfig->RedisPort();
	m_db = sxconfig->RedisDB();
#else
	strncpy(m_host, "127.0.0.1", 15);
	strncpy(m_pwd, "hugo123456", 64);
	m_port = 6379;
	m_db = 0;
#endif
	m_redisContext = nullptr;
	Connection();
}

Redis::~Redis() {
	if (m_redisContext != nullptr) {
		redisFree(m_redisContext);
		m_redisContext = nullptr;
	}
}

bool Redis::Connection(void) {
	if (m_redisContext != nullptr) {
		redisFree(m_redisContext);
		m_redisContext = nullptr;
	}

	redisReply* reply = nullptr;
	struct timeval timeout = {REDIS_TIMEOUT_SEC, REDIS_TIMEOUT_MICR};
	m_redisContext = redisConnectWithTimeout(m_host, m_port, timeout);
	if (m_redisContext == nullptr) {
		XERROR("Connection error: can't allocate redis context\n");

		return false;
	} else if (m_redisContext->err) {
		XERROR("ERROR: %s\n", m_redisContext->errstr);
		redisFree(m_redisContext);
		m_redisContext = nullptr;

		return false;
	} else {
		if (m_pwd != nullptr) {
			reply = (redisReply*)redisCommand(m_redisContext,"AUTH %s", m_pwd);
			if (strcmp(reply->str, "OK") != 0) {
				XERROR("ERROR: %s\n", reply->str);
				freeReplyObject(reply);
				reply = nullptr;

				return false;
			}
			freeReplyObject(reply);
			reply = nullptr;
		}

		if (m_db != 0) {
			reply = (redisReply*)redisCommand(m_redisContext,"SELECT %s", m_db);
			if (strcmp(reply->str, "OK") != 0) {
				XERROR("ERROR: %s\n", reply->str);
				freeReplyObject(reply);
				reply = nullptr;

				return false;
			}
			freeReplyObject(reply);
			reply = nullptr;
		}
	}

	return true;
}

bool Redis::IsConnect(void) {
	if (m_redisContext == nullptr) {
		return false;
	}

	redisReply* reply = nullptr;
	reply = (redisReply*)redisCommand(m_redisContext,"PING");
	if (strcmp(reply->str, "OK") != 0) {
		XERROR("ERROR: %s, Reconnect...\n", reply->str);
		freeReplyObject(reply);
		reply = nullptr;

		return false;
	}

	return true;
}

bool Redis::SelectDB(int db) {
	m_db = db;
	if (!IsConnect()) {
		bool flag = Connection();

		if (!flag) {
			XERROR("ERROR: SELECT %d fail!\n");

			return false;
		}
	}

	redisReply* reply = nullptr;
	reply = (redisReply*)redisCommand(m_redisContext,"PING");
	if (strcmp(reply->str, "OK") != 0) {
			XERROR("ERROR: SELECT %d fail: %s\n", db, reply->str);

			return false;
	}

	return true;
}

std::string Redis::HGet(const char* key, const char* field) {
	std::string ret;
#if 0
	if (!IsConnect()) {
		bool flag = Connection();

		if (!flag) {
			XERROR("ReConnect fail!\n");

			return ret;
		}
	}
#endif
	const char* argv[3];
	size_t argvLen[3];
	argv[0] = "HGET";
	argvLen[0]=4;
	argv[1] = key;
	argvLen[1] = strlen(key);
	argv[2] = field;
	argvLen[2] = strlen(field);
	redisReply* reply =(redisReply*) redisCommandArgv(m_redisContext, 3, argv, argvLen);

	if(reply->type != REDIS_REPLY_NIL){
		ret = std::string(reply->str,reply->str + reply->len);
	}
	freeReplyObject(reply);

	return ret;
}

/*return value
 * 1: create
 * 0: update
 * */
int Redis::HSet(const char* key, const char* field, const char* value) {
#if 0
	if (!IsConnect()) {
		bool flag = Connection();

		if (!flag) {
			XERROR("ReConnect fail!\n");

			return REDIS_REPLY_ERROR;
		}
	}
#endif
	redisReply* reply = (redisReply*)redisCommand(m_redisContext, "HSET %s %s %s", key, field, value);
	if (reply->type == REDIS_REPLY_INTEGER) {
		if (reply->integer == 1) {
			freeReplyObject(reply);

			return 1;
		} 

		if (reply->integer == 0) {
			freeReplyObject(reply);

			return 0;
		} 
	}

	if (reply->type == REDIS_REPLY_ERROR) {
		XERROR("HSET ERROR: %s\n", reply->str);
		freeReplyObject(reply);

		return REDIS_REPLY_ERROR;
	}

	freeReplyObject(reply);

	return REDIS_REPLY_ERROR;
}

/*this for binary set*/
int Redis::HSet(const char* key, const char* field, const char* value, size_t valueLen) {
#if 0
	if (!IsConnect()) {
		bool flag = Connection();

		if (!flag) {
			XERROR("ReConnect fail!\n");

			return -1;
		}
	}
#endif
	const char* argv[4];
	size_t argvLen[4];
	argv[0] = "HSET";
	argvLen[0] = 4;
	argv[1] = key;
	argvLen[1] = strlen(key);
	argv[2] = field;
	argvLen[2] = strlen(field);
	argv[3] = value;
	argvLen[3] = valueLen;
	redisReply * reply =(redisReply*) redisCommandArgv(m_redisContext, 4, argv, argvLen);
	if (reply->type == REDIS_REPLY_INTEGER) {
		if (reply->integer == 1) {
			freeReplyObject(reply);

			return 1;
		} 

		if (reply->integer == 0) {
			freeReplyObject(reply);

			return 0;
		} 
	}

	if (reply->type == REDIS_REPLY_ERROR) {
		XERROR("HSET ERROR: %s\n", reply->str);
		freeReplyObject(reply);

		return REDIS_REPLY_ERROR;
	}

	freeReplyObject(reply);

	return REDIS_REPLY_ERROR;
}

/*return value:
 * 1: success
 * 0: fail(key not exist)
 * -1: other error
 * */
int Redis::Del(const char* key) {
#if 0
	if (!IsConnect()) {
		bool flag = Connection();

		if (!flag) {
			XERROR("ReConnect fail!\n");

			return -1;
		}
	}
#endif
	redisReply* reply = (redisReply*)redisCommand(m_redisContext, "DEL %s", key);
	if (reply->type == REDIS_REPLY_INTEGER) {
		if (reply->integer == 1) {
			freeReplyObject(reply);

			return 1;
		} 

		if (reply->integer == 0) {
			freeReplyObject(reply);

			return 0; 
		} 
	} 

	if (reply->type == REDIS_REPLY_ERROR) {
		XERROR("ERROR: %s\n", reply->str);
		freeReplyObject(reply);

		return REDIS_REPLY_ERROR;
	}
	freeReplyObject(reply);

	return REDIS_REPLY_ERROR;
}

int Redis::ExistsKey(const char* key) {
#if 0
	if (!IsConnect()) {
		bool flag = Connection();

		if (!flag) {
			XERROR("ReConnect fail!\n");

			return -1;
		}
	}
#endif
	redisReply* reply = (redisReply*)redisCommand(m_redisContext, "EXISTS %s", key);
	if (reply->type == REDIS_REPLY_INTEGER) {
		if (reply->integer == 1) {
			freeReplyObject(reply);

			return 1;
		} 

		if (reply->integer == 0) {
			freeReplyObject(reply);

			return 0;
		} 
	}

	if (reply->type == REDIS_REPLY_ERROR) {
		XERROR("ERROR: %s\n", reply->str);
		freeReplyObject(reply);

		return REDIS_REPLY_ERROR;
	}
	freeReplyObject(reply);

	return REDIS_REPLY_ERROR;
}
}
