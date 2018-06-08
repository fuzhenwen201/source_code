#ifndef GROUP_SPREAD_H
#define GROUP_SPREAD_H

#include <sqlite3.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include "tnode.h"
#include <string>
#include "xlogger.h"
#include "tradingerrno.h"
#include <vector>

extern const uint32_t  symbol_size;
typedef struct _Symbol_DRIVE {
    uint32_t index;
    std::string symbol;
}SymbolDrive;

typedef struct _StructGroup {
    int32_t profit;
    int32_t minimum_amounts;
    int32_t maximum_amounts;
    int32_t bo_caltype;
    std::vector<uint32_t> condition;
}GroupStruct;

extern SymbolDrive symbol[];
extern SymbolDrive symbolBo[];
class UpdateGroupConsumer : public snetwork_xservice_tnode::TNodeConsumer{
public:
    UpdateGroupConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel);
    virtual ~UpdateGroupConsumer();

    std::string GetEventName();

    uint32_t EventHandle(u_int16_t tag, char *event, uint32_t eventlen);

    uint32_t ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send);

    int32_t SymbolInDrive(bool botrading, uint32_t id, uint32_t number);
    std::string GetSymbolfromDrive(bool botrading, uint32_t id, uint32_t number);

    int ReadGroupSymbolsList(bool botrading);
    int CalAskBidSpread(bool botrading, int &ask, int &bid, int totalspread);
    int  GetGroupSpread(bool botrading,uint32_t groupid, const char *symbol, GroupStruct &groupstr);
    void ShowGroupSymbolsList(bool botrading);
    int GetSupportSymbolist(std::string str);
    void ShowSymbolList();
private:
    std::map<uint32_t ,std::map<std::string,GroupStruct >> m_pGroupSymbolsList;
    std::map<uint32_t ,std::map<std::string,GroupStruct >> m_pGroupBoSymbolsList;
    std::string instance_id_;
    snetwork_xservice_tnode::TNodeChannel channel_;
};
#endif