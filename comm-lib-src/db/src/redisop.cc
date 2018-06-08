/*************************************************************************
	> File Name: redisop.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Mon 06 Mar 2017 07:42:04 PM CST
 ************************************************************************/

#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include"redisop.h"
#include"xlogger.h"
#include"redis_command_table.h"

int CRedis::m_sIndex = 0;
const char* CRedis::m_unLockCmd = "if redis.call('get', KEYS[1]) == ARGV[1] then return redis.call('del', KEYS[1]) else return 0 end";

CRedis::CRedis(const char* host, int port, int db, const char* pwd, int componentID, int retryCount, int retryDelay) {
	m_index = ++m_sIndex;
	memset(m_host, 0, sizeof(m_host));
	memset(m_pwd, 0, sizeof(m_pwd));
	strncpy(m_host, host, 15);
	if (pwd != nullptr) {
		strncpy(m_pwd, pwd, 64);
	}
	m_port = port;
	m_db = db;
	m_componentID = componentID;
	m_retryCount = retryCount;
	m_retryDelay = retryDelay;
	m_redisContext = nullptr;

	Connection();
}

CRedis::CRedis() {
	memset(m_host, 0, sizeof(m_host));
	memset(m_pwd, 0, sizeof(m_pwd));
	strncpy(m_host, "127.0.0.1", 15);
	strncpy(m_pwd, "hugo123456", 64);
	m_port = 6379;
	m_db = 0;
	m_redisContext = nullptr;
	Connection();
}

CRedis::~CRedis() {
	if (m_redisContext != nullptr) {
		redisFree(m_redisContext);
		m_redisContext = nullptr;
	}
}

bool CRedis::Connection(void) {
	if (IsConnect()) {
		return true;
	}

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
		if (strlen(m_pwd) != 0) {
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
			reply = (redisReply*)redisCommand(m_redisContext,"SELECT %d", m_db);
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

bool CRedis::IsConnect(void) {
	if (m_redisContext == nullptr) {
		return false;
	}

	redisReply* reply = nullptr;

	reply = (redisReply*)redisCommand(m_redisContext,"PING");
	if (strcmp(reply->str, "PONG") != 0) {
		XERROR("ERROR: %s, Reconnect...\n", reply->str);
		freeReplyObject(reply);
		reply = nullptr;

		return false;
	}

	freeReplyObject(reply);

	return true;
}

bool CRedis::SelectDB(int db) {
	if(m_db == db) {
		return true;
	}

	if (!IsConnect()) {
		bool flag = Connection();

		if (!flag) {
			XERROR("ERROR: SELECT %d fail!\n", db);

			return false;
		}
	}

	redisReply* reply = (redisReply*)redisCommand(m_redisContext,"SELECT %d", db);
	if (strcmp(reply->str, "OK") != 0) {
		XERROR("ERROR: %s\n", reply->str);
		freeReplyObject(reply);
		reply = nullptr;

		return false;
	}
	freeReplyObject(reply);
	reply = nullptr;

	reply = (redisReply*)redisCommand(m_redisContext,"PING");
	if (strcmp(reply->str, "PONG") != 0) {
		freeReplyObject(reply);
		XERROR("ERROR: SELECT %d fail: %s\n", db, reply->str);

		return false;
	}
	freeReplyObject(reply);
	m_db = db;

	return true;
}

std::string CRedis::HGet(const char* key, const char* field) {
	std::string ret;
	const char* argv[3];
	size_t argvLen[3];
	argv[0] = "HGET";
	argvLen[0]=4;
	argv[1] = key;
	argvLen[1] = strlen(key);
	argv[2] = field;
	argvLen[2] = strlen(field);
	redisReply* reply =(redisReply*) redisCommandArgv(m_redisContext, 3, argv, argvLen);

	if(reply->type != REDIS_REPLY_NIL) {
		ret = std::string(reply->str,reply->str + reply->len);
	}
	freeReplyObject(reply);

	return std::move(ret);
}

/*return value
 * 1: create
 * 0: update
 * */
int CRedis::HSet(const char* key, const char* field, const char* value) {
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

	int ret = reply->integer;
	freeReplyObject(reply);

	return ret;
}

void FreeReplyObject(redisReply* p) {
	freeReplyObject(p);
}

std::shared_ptr<redisReply> CRedis::Excute(enum RedisCom comIndex, int arc, const char** arv) {
	redisReply* reply = nullptr;
	int size = arc + 1;
	const char** argv = new const char*[size];
	size_t* argvLen = new size_t[size];
	argv[0] = RedisComName[comIndex];
	argvLen[0] = strlen(RedisComName[comIndex]);
	for (int i=0; i<arc; ++i) {
		argv[i+1] = arv[i];
		argvLen[i+1] = strlen(arv[i]);
	}

	reply = (redisReply*) redisCommandArgv(m_redisContext, size, argv, argvLen);
	
	delete[] argv;
	delete[] argvLen;

	return std::shared_ptr<redisReply>(reply, FreeReplyObject);
}

std::shared_ptr<redisReply> CRedis::Excute(const char* format, ...) {
	char* str = nullptr;
	va_list ar;
	va_start(ar, format);
	vasprintf(&str, format, ar);
	va_end(ar);

	redisReply* reply = nullptr;
	reply =(redisReply*) redisCommand(m_redisContext, str);

	free(str);
	str = nullptr;

	return std::shared_ptr<redisReply>(reply, FreeReplyObject);
}

///* this for binary  */
int CRedis::HSet(const char* key, const char* field, const char* value, size_t valueLen) {
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
int CRedis::Del(const char* key) {
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

int CRedis::ExistsKey(const char* key) {
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

long CRedis::GetUniqueID(void) const {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (((tv.tv_sec * 1000 + tv.tv_usec) << 16) | (m_componentID<<5) | m_index);
}

bool CRedis::Lock(CRedisLock& lk) {
	long uniq = GetUniqueID();
	lk.Value(std::to_string(uniq).c_str());
	int retryCount = 0;
	redisReply* reply = nullptr; 
	char* cmd = nullptr;
	asprintf(&cmd, "SET %s %s PX %d NX", lk.Key(), lk.Value(), lk.ValidityTime());
	reply = (redisReply*)redisCommand(m_redisContext, cmd);

	if (reply->str && strcmp(reply->str, "OK") == 0) {
			freeReplyObject(reply);
			free(cmd);
			cmd = nullptr;

			return true;
	}

	free(cmd);
	cmd = nullptr;
	freeReplyObject(reply);

	return false;
}

bool CRedis::UnLock(CRedisLock& lk) {
	const char* cmd[] = {
		m_unLockCmd,
		"1",
		lk.Key(),
		lk.Value(),
	};
	std::shared_ptr<redisReply> reply = Excute(EVAL, 4, cmd);

	if (reply.get()!= nullptr) {
		if (reply->type == REDIS_REPLY_INTEGER) {
			if (reply->integer == 1) {
				return true;
			} 
		}
	}

	return false;
}
/* CRedis end **/

/* CRedisLock begin */
CRedisLock::CRedisLock(const char* key, const char* value, int validityTime) :
	m_key(nullptr),
	m_value(nullptr),
	m_validityTime(validityTime) {
	if (key != nullptr) {
		m_key = strdup(key);
	}

	if (value != nullptr) {
		m_value = strdup(value);
	}
}

CRedisLock::~CRedisLock() {
	if (m_key != nullptr) {
		free(m_key);
		m_key = nullptr;
	}

	if (m_value != nullptr) {
		free(m_value);
		m_value = nullptr;
	}
}

/* CRedisLock end */
