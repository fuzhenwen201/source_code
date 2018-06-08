/*************************************************************************
    > File Name: DataStorage.h
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Wed 24 Jan 2018 04:10:24 PM CST
 ************************************************************************/
#ifndef __DATA_STORAGE_H_
#define __DATA_STORAGE_H_

#include <string>
#include <iostream>
#include <map>
#include "tnode.h"
#include <pthread.h>
#include "config.h"
#include "xlogger.h"
#include "ME.Interface.pb.h"
#include <hiredis/hiredis.h>

using std::string;
struct EntrustStruct {
    char strordId[35];
    uint64_t lsubmitTime;
    uint32_t uctransType;
    uint32_t ucordType;
    uint32_t uiuserId;
    uint32_t uiexchobjId;
    int64_t lprice;
    int64_t lnumber;
    uint32_t uiordIndex;
	uint32_t ucsubmitype;
};

struct DealOrdSruct {
    char bidorderid[35];
    char askorderid[35];
    uint32_t biduserid;
	uint32_t askuserid;
    int64_t price ;
    int64_t number;
    uint32_t exchtype;
    uint32_t exchobjid;
    uint64_t mebidordid;
    uint64_t measkordid;
};

bool SendLog2Server(uint32_t loglevel, std::string text, snetwork_xservice_tnode::TNode* tnode);

class EntrustOrdConsumer : public snetwork_xservice_tnode::TNodeConsumer{
public:
    EntrustOrdConsumer(const char *bindingkey, const char *queuename);
    ~EntrustOrdConsumer();

    std::string GetQueueName();
    std::string GetBindingkey();

    uint32_t ConsumerData(char* pMsg, int32_t nMsgLen);

private:
    std::string m_bindingkey;
    std::string m_queuename;
};

class DealOrderConsumer : public snetwork_xservice_tnode::TNodeConsumer{
public:
    DealOrderConsumer(const char *bindingkey, const char *queuename);
    ~DealOrderConsumer();

    std::string GetQueueName();
    std::string GetBindingkey();

    uint32_t ConsumerData(char* pMsg, int32_t nMsgLen);

private:
    std::string m_bindingkey;
    std::string m_queuename;
};

class DataStorageService {
public:
    DataStorageService();

    ~DataStorageService();

    void Init(int argc, char** argv);
    void Run();
};

#endif
