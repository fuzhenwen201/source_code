#include "../include/redis.h"
#include <unistd.h>

void connectCallback(const redisAsyncContext *c, int status)
{
    if (status != REDIS_OK)
    {
        LOG_ERR("Error: %s", c->errstr);
        return;
    }
    
    LOG_INFO("Connected success...");
}

void disconnectCallback(const redisAsyncContext *c, int status)
{
    if (status != REDIS_OK)
    {
        LOG_ERR("Error: %s", c->errstr);
        return;
    }
    LOG_ERR("DisConnected  ...");
}


CRedis::CRedis()
{

}

CRedis::~CRedis()
{
	//if (_connect != NULL)
		//redisFree(_connect);
    this->_connect = NULL;
    this->_reply = NULL;
}

bool CRedis::connect(std::string host, int port, int timeout, std::string strPwd, std::string strRdsins)
{
    //redisConnectWithTimeout(const char *ip, int port, const struct timeval tv);

	if (host == "" || port < 1024 || timeout < 0)
    {
        LOG_ERR( "invalid parameter");
        return FAILE;
    }

    struct timeval tv = {0};
    tv.tv_sec = timeout;

    timeout == 0 ?
    this->_connect = redisConnect(host.c_str(), port) :
                     this->_connect = redisConnectWithTimeout(host.c_str(), port, tv);

	if (strPwd.empty() && this->_connect != NULL && !this->_connect->err)
	{
		LOG_DEBUG("do not need auth!");
		return SUCESS;
	}

	LOG_DEBUG("begin to auth!");
    if(this->_connect != NULL && !this->_connect->err)
    {
        strRdsins == "" ?
        this->_reply = (redisReply *)redisCommand(this->_connect, "AUTH %s", strPwd.c_str()) :
                       this->_reply = (redisReply *)redisCommand(this->_connect, "AUTH %s %s", strRdsins.c_str(), strPwd.c_str());
        if(strcasecmp(_reply->str, "OK") == 0)
        {
            freeReplyObject(_reply);
            LOG_INFO( "auth sucess");
            return SUCESS;
        }
        freeReplyObject(_reply);
        //LOG_INFO( "connect SUCESS");

    }
    LOG_ERR( "auth error: %s\n", this->_connect->errstr);
    return FAILE;
}
bool CRedis::asynconnect(std::string host, int port, int timeout, std::string strPwd, std::string strRdsins,
                         struct event_base *base)
{
    //if(strPwd == "" || host == "" || port < 1024 || timeout < 0)
	if (host == "" || port < 1024 || timeout < 0)
    {
        LOG_ERR( "invalid parameter");
        return FAILE;
    }
    _asynconnect = redisAsyncConnect(host.c_str(), port);
    redisLibeventAttach(_asynconnect, base);
    redisAsyncSetConnectCallback(_asynconnect, connectCallback);
    redisAsyncSetDisconnectCallback(_asynconnect, disconnectCallback);

	if (!strPwd.empty())
	{
		redisAsyncCommand(_asynconnect, NULL, NULL, "AUTH %s", strPwd.c_str());
	}
    //

}

bool CRedis::asyncsubscribe(std::string key, Fun fn,void* param)
{
    redisAsyncCommand(_asynconnect, fn, (char *) param, "SUBSCRIBE %s", key.c_str());
}

bool CRedis::asyncpsubscribe(std::string key, Fun fn, void* param)
{
	LOG_DEBUG("regist cmd!");
	redisAsyncCommand(_asynconnect, fn, (char *)param, "PSUBSCRIBE %s", key.c_str());
}

std::string CRedis::get(std::string key)
{
    this->_reply = (redisReply *)redisCommand(this->_connect, "GET %s", key.c_str());
    string strRet = "";
	if (this->_reply == NULL)
	{
		LOG_ERR("get key : [%s] error! ", key.c_str());
		return strRet;
	}
    //
    if( !(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str, "OK") == 0))
    {
		LOG_DEBUG("debug info, key[%s] LEN[%d],value[%s]", key.c_str(), _reply->len, _reply->str);
        if(_reply->len > 0)
        {
            LOG_DEBUG("key[%s] LEN[%d]", key.c_str(),_reply->len);
            strRet = _reply->str;
        }
    }
    else
    {
        LOG_ERR("Execut command1 failure\n");
    }

    freeReplyObject(_reply);
    return strRet;
}

void CRedis::set(std::string key, std::string value)
{
    this->_reply = (redisReply *)redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
    freeReplyObject(_reply);
    LOG_DEBUG("set key[%s] value[%s]", key.c_str(), value.c_str());
    //pthread_mutex_unlock(&m_mutex);
}
void CRedis::set(std::string key, std::string value, int sec)
{
    this->_reply  = (redisReply *)redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
    freeReplyObject(_reply);
    this->_reply = (redisReply *)redisCommand(this->_connect, "EXPIRE %s %d", key.c_str(), sec);
    freeReplyObject(_reply);
}
void CRedis::hset(std::string key, std::string fild, std::string value)
{
    //LOG_INFO("key[%s] fild[%s] value[%s]", key.c_str(),  fild.c_str(), value.c_str());
    //pthread_mutex_lock(&m_mutex);
    this->_reply = (redisReply *)redisCommand(this->_connect,
                   "HSET %s %s %s", key.c_str(), fild.c_str(), value.c_str());

    freeReplyObject(_reply);
    //pthread_mutex_unlock(&m_mutex);
}
void CRedis::hset(std::string key, std::string fild, std::string value, int sec)
{
    LOG_DEBUG("EXPIRE key[%s] fild[%s] value[%s]", key.c_str(),  fild.c_str(), value.c_str());

    this->_reply = (redisReply *)redisCommand(this->_connect,
                   "HSET EXPIRE:%s %s %s", fild.c_str(), key.c_str(), value.c_str());
    freeReplyObject(_reply);
    this->_reply = (redisReply *)redisCommand(this->_connect, "EXPIRE EXPIRE:%s %d", fild.c_str(), sec);
    freeReplyObject(_reply);
}
std::string CRedis::hgetall(std::string key)
{
    this->_reply = (redisReply *)redisCommand(this->_connect, "HGETALL %s", key.c_str());
    string strRet = "";

    //LOG_INFO("key[%s] LEN[%d]", key.c_str(),  _reply->len);
    if( !(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str, "OK") == 0))
    {
        if(_reply->len != 0)
        {
            LOG_DEBUG("key[%s] LEN[%d]", key.c_str(),  _reply->len);
            strRet = _reply->str;
        }
    }
    else
    {
        LOG_ERR("Execut command1 failure\n");
    }

    freeReplyObject(_reply);
    return strRet;
}

std::string CRedis::hget(std::string key, std::string fild)
{
    this->_reply = (redisReply *)redisCommand(this->_connect, "HGET %s %s", key.c_str(), fild.c_str());
    string strRet = "";

    //LOG_INFO("key[%s] LEN[%d]", key.c_str(),  _reply->len);
    if( !(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str, "OK") == 0))
    {
        if(_reply->len != 0)
        {
            LOG_ERR("key[%s] LEN[%d]", key.c_str(),  _reply->len);
            strRet = _reply->str;
        }
    }
    else
    {
        LOG_ERR("Execut command1 failure\n");
    }

    freeReplyObject(_reply);
    return strRet;
}
void CRedis::del(std::string key)
{
    //pthread_mutex_lock(&m_mutex);
    this->_reply  = (redisReply *)redisCommand(this->_connect, "DEL %s", key.c_str());
    freeReplyObject(_reply);
    //pthread_mutex_unlock(&m_mutex);
    LOG_DEBUG("DEL key[%s]", key.c_str());

}

void CRedis::hdel(std::string key, std::string fild)
{
    this->_reply  = (redisReply *)redisCommand(this->_connect, "HDEL %s %s", key.c_str(), fild.c_str());
    freeReplyObject(_reply);
    LOG_DEBUG("HDEL key[%s] fild[%s]", key.c_str(), fild.c_str());
}
int CRedis::getkeyssize(std::string key)
{
    this->_reply = (redisReply *)redisCommand(this->_connect, "KEYS %s", key.c_str());
    int keyssize = 0;
    //LOG_INFO("key[%s] elements[%d]", key.c_str(),  _reply->elements);
    if( !(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str, "OK") == 0))
    {
        LOG_DEBUG("key[%s] elements[%d]", key.c_str(),  _reply->elements);
        keyssize = _reply->elements;
    }
    else
    {
        LOG_ERR("Execut command1 failure\n");
    }

    freeReplyObject(_reply);
    return keyssize;
}

void CRedis::setexpire(std::string key, int sec)
{
    LOG_DEBUG("EXPIRE key[%s]", key.c_str());
    this->_reply  = (redisReply *)redisCommand(this->_connect, "EXPIRE %s %d", key.c_str(), sec);
    freeReplyObject(_reply);
}

void CRedis::getkeyfield(std::string key, std::vector<string> &fieldvactor)
{
    LOG_DEBUG("field key[%s]", key.c_str());
    this->_reply = (redisReply *)redisCommand(this->_connect, "HKEYS %s", key.c_str());
    if(!(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str, "OK") == 0))
    {
        for(size_t i = 0; i < _reply->elements; i++)
        {
            fieldvactor.push_back(_reply->element[i]->str);
            //LOG_INFO("value[%s]", fieldvactor[i].c_str());
        }
    }
    freeReplyObject(_reply);
}


