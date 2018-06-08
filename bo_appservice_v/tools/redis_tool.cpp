#include <stdio.h>
#include <unistd.h>
#include "../include/redis.h"
#include "../include/const.h"

char* g_RdsPort;
char* g_RdsIp;
char* g_RdsTimeout;
char* g_RdsPassword;
char* g_RdsIns;
class CRedis rds;

int localInit()
{
	if (FAILE == InitLog((char *)"redis_tool", (char*)"0"))
	{
		printf("init log error\n");
		return -2;
	}

	//初始化redis的IP和端口
	M_XRETURN(((g_RdsIp = getenv("RDS_IP")) == NULL),
		false, LOG_ERR("get env RDS_IP err!"));

	M_XRETURN(((g_RdsPort = getenv("RDS_PORT")) == NULL),
		false, LOG_ERR("get env RDS_PORT err!"));

	M_XRETURN(((g_RdsTimeout = getenv("RDS_CON_TIMEOUT")) == NULL),
		false, LOG_ERR("get env RDS_CON_TIMEOUT err!"));

	M_XRETURN(((g_RdsPassword = getenv("RDS_PWD")) == NULL),
		false, LOG_ERR("get env RDS_PWD err!"));

	g_RdsIns = getenv("RDS_INS");

	printf("init redis ip[%s],por[%s],time[%s],pwd[%s],ins[%s]\n", g_RdsIp, g_RdsPort, g_RdsTimeout, g_RdsPassword, g_RdsIns);

	if (int nRet = rds.connect(g_RdsIp, atoi(g_RdsPort), atoi(g_RdsTimeout), g_RdsPassword, g_RdsIns == NULL ? "" : g_RdsIns))
	{
		printf("connect redis error[%d]\n", nRet);
		return -1;
	}

	return 0;
}

void useage()
{
	printf("Usage: -s key value set a key-value into redis\n");
	printf("Usage: -f filepath set a key-value with file-content into redis\n");
	printf("Usage: -g key get the key's value\n");
	printf("Usage: -d key delete key value from redis\n");
}

int judgeArgc(int argc, int need)
{
	if (argc < need)
	{
		useage();
		return -1;
	}
	return 0;
}

int redisSet(char *key, char *value)
{
	if (key == NULL || value == NULL)
	{
		printf("key value should not null\n");
		return -1;
	}
	rds.set(key, value);
}


int setWithFile(const char *path)
{
	FILE *fp = NULL;
	char line[102400];
	memset(line, 0x00, sizeof(line));

	if ((fp = fopen(path, "r")) == NULL)
	{
		printf("open file error,%s",path);
		return FAILE;
	}

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		if (line[0] == '#')
		{
			continue;
		}


		int len = strlen(line);
		//printf("asdasdline :%s,len:%d,%c %c\n", line,len, line[0],line[len - 2]);
		//去掉尾部的 \n 
		if (line[0] == '[' && line[len - 2] == ']')
		{
			char key[128] = { 0 };
			line[len - 2] = '\0';
			memcpy(key, line + 1, len - 3); // 去掉 [ ] \n
			printf("get a key: %s\n", key);

			memset(line, 0x00, sizeof(line));
			while ((fgets(line, sizeof(line), fp)) != NULL)
			{
				if (line[0] == '[')
				{
					break;
				}
				else if (memcmp(line, "VALUE=", 6) == 0)
				{
					line[strlen(line) - 1] = '\0';
					string value = line + 6;

					printf("get a key: %s,value:%s\n", key, value.c_str());

					rds.set(key, value);

					break;
				}
				memset(line, 0x00, sizeof(line));
			}
		}
	}
	if (fp != NULL)
	{
		fclose(fp);
	}
	printf("set with file end \n");
	return SUCESS;
}


int main(int argc, char *argv[])
{
	if (judgeArgc(argc, 2) != 0)
	{
		return -1;
	}

	int nRet = 0;
	if ((nRet = localInit()) != 0)
	{
		printf("local init failed");
		return -1;
	}

	int ch;
	// 用Getopt在Linux下获取命令行参数的方法 [摘要] 如果getopt()找不到符合的参数则会印出错信息，
	// 并将全域变量optopt设为“?”字符，如果不希望getopt()印出错信息，则只要将全域变opterr设为0即可
	opterr = 0;

	while ((ch = getopt(argc, argv, "hf:s:g:d:")) != EOF)
	{
		switch (ch)
		{
		case 's':
		{
			if (judgeArgc(argc, 4) != 0)
				return -1;

			rds.set(argv[2], argv[3]);
			printf("set->your save key:%s, value: %s\n", argv[2], rds.get(argv[2]).c_str());
			break;
		}
		case 'f':
		{
			if (judgeArgc(argc, 3) != 0)
				return -1;

			setWithFile(argv[2]);

			break;
		}
		case 'g':
		{
			if (judgeArgc(argc, 3) != 0)
				return -1;
			char *key = argv[2];
			printf("get from redis your key:%s, value: %s\n", key, rds.get(key).c_str());
			break;
		}
		case 'd':
		{
			if (judgeArgc(argc, 3) != 0)
				return -1;
			char *key = argv[2];
			rds.del(key);
			printf("delete from redis key:%s, value: %s\n", key, rds.get(key).c_str());
			break;
		}
		case 'h':
		case '?':
		default:
		{
			useage();
			printf("illegal option: %c\n", ch);
			break;
		}
		}
	}
	return 0;
}
