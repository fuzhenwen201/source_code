#include "../include/worknew.h"
#include "../include/redis/hiredis.h"
#include "../include/redis/async.h"
#include "../include/redis/libevent.h"

using namespace std;

char *g_RdsPort;
char *g_RdsIp;
char *g_RdsTimeout;
char *g_RdsPassword;
char *g_RdsIns;

redisContext* _connect;
redisReply* _reply;
redisAsyncContext* _asynconnect;

long get_tm_usec()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	cout << "second = " << tv.tv_sec << endl;
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

bool connect(std::string host, int port, int timeout, std::string strPwd, std::string strRdsins)
{
	//redisConnectWithTimeout(const char *ip, int port, const struct timeval tv);

	if (strPwd == "" || host == "" || port < 1024 || timeout < 0)
	{
		cout << "invalid parameter" << endl;
		return FAILE;
	}

	struct timeval tv = { 0 };
	tv.tv_sec = timeout;

	timeout == 0 ?
		_connect = redisConnect(host.c_str(), port) :
		_connect = redisConnectWithTimeout(host.c_str(), port, tv);

	if (_connect != NULL && !_connect->err)
	{
		strRdsins == "" ?
			_reply = (redisReply *)redisCommand(_connect, "AUTH %s", strPwd.c_str()) :
			_reply = (redisReply *)redisCommand(_connect, "AUTH %s %s", strRdsins.c_str(), strPwd.c_str());
		if (strcasecmp(_reply->str, "OK") == 0)
		{
			cout << "auth sucess" << endl;
			return SUCESS;
		}
		//LOG_INFO( "connect SUCESS");

	}
	cout  << "auth error:" << _connect->errstr << endl;
	return FAILE;
}

int getkeyssize(std::string key, vector<std::string> &vecKey)
{
	_reply = (redisReply *)redisCommand(_connect, "KEYS %s", key.c_str());
	int keyssize = 0;

	 printf("type: %d\n", _reply->type);
     switch(_reply->type) {
     case REDIS_REPLY_STATUS:
         printf("type:%s, reply->len:%d reply->str:%s\n", "REDIS_REPLY_STATUS", _reply->len, _reply->str);
         break;
     case REDIS_REPLY_ERROR:
         printf("type:%s, reply->len:%d reply->str:%s\n", "REDIS_REPLY_ERROR", _reply->len, _reply->str);
        break;
     case REDIS_REPLY_INTEGER:
         printf("type:%s, reply->integer:%lld\n", "REDIS_REPLY_INTEGER", _reply->integer);
         break;
     case REDIS_REPLY_NIL:
         printf("type:%s, no data\n", "REDIS_REPLY_NIL");
         break;
     case REDIS_REPLY_STRING:
         printf("type:%s, reply->len:%d reply->str:%s\n", "REDIS_REPLY_STRING", _reply->len, _reply->str);
        break;
     case REDIS_REPLY_ARRAY:
         printf("type:%s, reply->elements:%d\n", "REDIS_REPLY_ARRAY", _reply->elements);
         for (int i = 0; i < _reply->elements; i++) {
             printf("%d: %s\n", i, _reply->element[i]->str);
			 vecKey.push_back(_reply->element[i]->str);
         }
         break;
     default:
        printf("unkonwn type:%s\n", _reply->type);
         break;
     }

	/*if (!(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str, "OK") == 0))
	{
		cout << "key:" << key.c_str() << "size =" << _reply->elements << ",value=" << _reply->str << endl;
		keyssize = _reply->elements;
	}
	else
	{
		cout << "Execut command1 failure" << endl;
	}*/

	freeReplyObject(_reply);
	return keyssize;
}

std::string get(std::string key)
{
	_reply = (redisReply *)redisCommand(_connect, "GET %s", key.c_str());
	std::string strRet = "";
	if (_reply == NULL)
		return strRet;
	//
	if (!(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str, "OK") == 0))
	{
		if (_reply->len > 0)
		{
			//cout << "key =" << key.c_str() << ", str=" << _reply->str << endl;
			strRet = _reply->str;
		}
	}
	else
	{
		cout << "Execut command1 failure" << endl;
	}

	freeReplyObject(_reply);


	return strRet;
}


int main(int argc, char *const argv[])
{
	//初始化redis的IP和端口

	if (argc < 1)
	{
		cout << "bad input param!" << endl;
		return -1;
	}

	//g_RdsPort = getenv("RDS_PORT");
	//g_RdsIp = getenv("RDS_IP");
	g_RdsTimeout = getenv("RDS_CON_TIMEOUT");
	//g_RdsPassword = getenv("RDS_PWD");
	g_RdsIns = getenv("RDS_INS");

	char szIp[] = {"r-wz968b39d0154bb4.redis.rds.aliyuncs.com"};
	char szPort[] = {"6379"};
	char szPwd[] = {"ma7Vai2CMAFd758R"};

	int nCount = 0;
	cout << "ip=" << szIp << ", port=" << szPort << ",pwd=" << szPwd << endl;

	int nRet = 0;

	/*if (nRet = connect(g_RdsIp, atoi(g_RdsPort), atoi(g_RdsTimeout), g_RdsPassword, g_RdsIns == NULL ? "" : g_RdsIns))
	{
		cout << "connect rds error!" << endl;
		return FAILE;
	}*/


	if (nRet = connect(szIp, atoi(szPort), atoi(g_RdsTimeout), szPwd, g_RdsIns == NULL ? "" : g_RdsIns))
	{
		cout << "connect rds error!" << endl;
		return FAILE;
	}

	vector<std::string> vecValue;
	std::string tmp = argv[1];
	std::string strKey = tmp + "*com";

	cout << strKey << endl;

	getkeyssize(strKey, vecValue);

	vector < std::string > ::iterator it = vecValue.begin();
	for (; it != vecValue.end(); it ++)
	{
		std::string key = *it;
		std::string value = get(key);
		//cout << "value=" << value << endl;
		Json::Reader rd;
		Json::Value  va;

		if (!rd.parse(value.c_str(), va))
		{
			cout << "bad json ! va:" << value << endl;
		}
		std::string token = va["app_token"].asString();

		cout << "token = " << token << endl;
		
		int len = token.length();

		char *p = (char *)token.c_str();

		p = p + (len - 16);

		long old = strtoll(p, NULL, 10);
		long now = get_tm_usec();

		cout << "time is : " << old << ", now is : " << now << endl;

		long sec = (now - old) / 1000000;
		int day = sec / (24 * 60 * 60);

		cout << "sec:" << sec << "day:" << day << endl;

		if (day <= 30)
		{
			nCount++;
		}

	}

	cout << "online num: " << nCount << ", total num: " << vecValue.size() << endl;

	return 0;

	



}