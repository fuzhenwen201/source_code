/*************************************************************************
	> File Name: redis_command_table.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Mon 12 Feb 2018 05:36:58 PM CST
 ************************************************************************/
#ifndef _REDIS_COMMAND_TABLE_H_
#define _REDIS_COMMAND_TABLE_H_

#ifdef __cpluscplus
extern "C" {
#endif
#define REDIS_COMMAND_TABLE \
X(EVAL, "EVAL") \
X(DISCARD, "DISCARD") \
X(EXEC, "EXEC") \
X(MULTI, "MULTI") \
X(UNWATCH, "UNWATCH") \
X(WATCH, "WATCH") \
X(INCR, "INCR") \
X(INCRBY, "INCRBY") \
X(DECR, "DECR") \
X(DECRBY, "DECRBY") \
X(DEL, "DEL") \
X(EXISTS, "EXISTS") \
X(SET, "SET") \
X(SETEX, "SETEX") \
X(SETNX, "SETNX") \
X(GET, "GET") \
X(GETSET, "GETSET") \
X(EXPIRE, "EXPIRE") \
X(EXPIREAT, "EXPIREAT") \
X(PEXPIRE, "PEXPIRE") \
X(PEXPIREAT, "PEXPIREAT") \
X(PERSIST, "PERSIST") \
X(TTL, "TTL") \
X(PTTL, "PTTL") \
X(KEYS, "KEYS") \
X(HSET, "HSET") \
X(HSETNX, "HSETNX") \
X(HGET, "HGET") \
X(HGETALL, "HGETALL") \
X(HINCRBY, "HINCRBY") \
X(HINCRBYFLOAT, "HINCRBYFLOAT") \
X(HKEYS, "HKEYS") \
X(HLEN, "HLEN") \
X(HMGET, "HMGET") \
X(HMSET, "HMSET") \
X(HSCAN, "HSCAN") \
X(HSTRLEN, "HSTRLEN") \
X(HVALS, "HVALS") \
X(HDEL, "HVALS") \
X(HEXISTS, "HEXISTS")

#define X(a, b) a,
enum RedisCom {
	REDIS_COMMAND_TABLE
};
#undef X

extern char* RedisComName[];

#ifdef __cpluscplus
}
#endif
#endif

