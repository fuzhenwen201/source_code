/*************************************************************************
	> File Name: redis_command_table.c
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Mon 12 Feb 2018 05:55:48 PM CST
 ************************************************************************/

#include"redis_command_table.h"
#define X(a, b) [a] = b,
char* RedisComName[] = {
	REDIS_COMMAND_TABLE
};
#undef X
