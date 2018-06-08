/*************************************************************************
    > File Name: Subscriptionmsg.h
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Wed 24 Jan 2018 04:10:24 PM CST
 ************************************************************************/
#ifndef __SUBSCRIPTION_SERVICE_H_
#define __SUBSCRIPTION_SERVICE_H_

#include <string>
#include <iostream>
#include <map>
#include "tnode.h"
#include <pthread.h>
#include "config.h"
#include "OrderIntervalMsg.pb.h"
#include "Orderexternal.h"
#include <hiredis/hiredis.h>
#include "xlogger.h"

using std::string;

extern pthread_cond_t g_cond;
extern pthread_mutex_t g_lock;
extern pthread_mutex_t g_nasymbolock;
extern snetwork_xservice_tnode::TNode *g_tnode;
extern redisContext *g_pRedisContext;
extern std::vector<std::string> g_hostnames;
extern std::map<std::string, std::string> stop_symbols;
extern char g_hostname[128];

struct StSigIpinfo
{
    uint32_t wIpType;       //IP类型，0表示IPV4 1表示IPV6
    std::string ucIpAddress;   //IP地址,如果是IPV4的话,则只需要获取前四个字节
};

struct StSymbolInfo
{
    uint32_t uiSymbolID;
    std::string uiSymbolName;
};

struct StUserLogonResp
{
    uint32_t ucLoginRsp;  //登陆结果， 0：表示鉴权通过，1表示用户或者密码错误//只有在鉴权通过的情况下，才会有IP信息和交易品种信息
    uint32_t uiUserID; //对应的用户注册ID
    uint32_t uiIPLisyNum;
    uint32_t uiSymbolNum; //交易品种的个数
    std::string chUserName;
    std::vector<StSigIpinfo> stIPInfo;//IP地址,如果是IPV4的话,则只需要获取前四个字节
    std::vector<StSymbolInfo> stSymbolInfo; //交易品种ID
};

struct RoutingAndTnode{
    snetwork_xservice_tnode::TNode* tnode;
    std::string routingKey1;
    std::string routingKey2;
};
extern std::map<uint32_t,RoutingAndTnode> userMaptnodes;
extern pthread_mutex_t userMaptnodes_lock;
extern std::map<std::string, StUserLogonResp> g_LogonResp;
extern std::map<uint32_t, FIX::SessionID> userIdMapSessionId;
extern std::map<uint32_t, std::vector<StSymbolInfo>> userIdMapSymbolArry;
extern pthread_mutex_t userIdMapSessionId_lock;
extern pthread_mutex_t userIdMapSymbolArry_lock;

bool SendLog2Server(uint32_t loglevel, std::string text, snetwork_xservice_tnode::TNode* tnode);

class LogonInfoConsumer : public snetwork_xservice_tnode::TNodeConsumer{
public:
    LogonInfoConsumer(const char *bindingkey, const char *queuename);
    ~LogonInfoConsumer();

    std::string GetQueueName();
    std::string GetBindingkey();

    uint32_t ConsumerData(char* pMsg, int32_t nMsgLen);

private:
    std::string m_bindingkey;
    std::string m_queuename;
};

class OrderRespConsumer : public snetwork_xservice_tnode::TNodeConsumer{
public:
    OrderRespConsumer(const char *bindingkey, const char *queuename);
    ~OrderRespConsumer();

    std::string GetQueueName();
    std::string GetBindingkey();

    uint32_t ConsumerData(char* pMsg, int32_t nMsgLen);

private:
    std::string m_bindingkey;
    std::string m_queuename;
};

class QueryRespConsumer : public snetwork_xservice_tnode::TNodeConsumer{
public:
    QueryRespConsumer(const char *bindingkey, const char *queuename);
    ~QueryRespConsumer();

    std::string GetQueueName();
    std::string GetBindingkey();

    uint32_t ConsumerData(char* pMsg, int32_t nMsgLen);

private:
    std::string m_bindingkey;
    std::string m_queuename;
};

class OrderService {
public:
    OrderService();

    ~OrderService();

    void Init(STTnodeConfig&, RedisConfigInfo&, std::string&);
    void Run();
};

#endif
