/*************************************************************************
    > File Name: ExtractData.h
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Wed 24 Jan 2018 04:10:24 PM CST
 ************************************************************************/
#ifndef __EXTRACT_DATA_H_
#define __EXTRACT_DATA_H_

#include <string>
#include <iostream>
#include <map>
#include "tnode.h"
#include <pthread.h>
#include "config.h"
#include "ME.Interface.pb.h"
#include "xlogger.h"
#include <hiredis/hiredis.h>

using std::string;
struct EntrustStruct {
    uint64_t SubOrderID;
    char UserOrderID[35];
    uint32_t UserId;
    uint64_t SubmitTime;
    uint32_t SubmitType;
    uint32_t OrderType;
    uint32_t TransType;
    uint32_t Symbol;
    uint64_t OrderPrice;
    uint64_t OrderNumber;
    uint32_t OrderIndex;
};

struct DealOrdSruct {
    uint64_t DealID;
    uint32_t UserId;
    uint32_t Symbol;
    uint32_t TransType;
    char UsrBidOrderId[35];
    char UsrAskOrderId[35];
    uint64_t DealPrice;
    uint64_t DealNum;
};

class RetransReqConsumer : public snetwork_xservice_tnode::TNodeConsumer {
public:
    RetransReqConsumer(const char *bindingkey, const char *queuename);
    ~RetransReqConsumer();
    std::string GetQueueName();
    std::string GetBindingkey();
    uint32_t ConsumerData(char *pMsg, int32_t nMsgLen);
private:
    std::string m_bindingkey;
    std::string m_queuename;
};

class ExtractDataService {
public:
    ExtractDataService();

    ~ExtractDataService();

    void Init(int argc, char** argv);
    void Run();

private:
    STTnodeConfig tnodecfg;
};

#endif
