#include "groupspread.h"
#include <rapidjson/document.h>

using std::string;
using std::map;
using std::pair;

int32_t UpdateGroupConsumer::SymbolInDrive(bool botrading, uint32_t id, uint32_t number){
    uint32_t i = 0;
    if (botrading){
        for (i = 0; i < number; i++) {
            if (symbolBo[i].index == id)
                return i;
        }
    } else  {
        for (i = 0; i < number; i++) {
            if (symbol[i].index == id)
                return i;
        }
    }
    return -1;
}

string UpdateGroupConsumer::GetSymbolfromDrive(bool botrading, uint32_t id, uint32_t number){
    uint32_t i = 0;
    string tmpsymbol;
    if(botrading){
        for(i = 0; i < number; i++){
            if(symbolBo[i].index == id) {
                tmpsymbol = symbolBo[i].symbol;
                return tmpsymbol;
            }
        }
    } else {
        for(i = 0; i < number; i++){
            if(symbol[i].index == id) {
                tmpsymbol = symbol[i].symbol;
                return tmpsymbol;
            }
        }
    }

    return tmpsymbol;
}

int ReadGroupSymbolsCallback(void* pList,int32_t nCount,char** pValue,char** pName){
    map<uint32_t, map<string, GroupStruct>> *pGroupSymbolsList = (map<uint32_t, map<string, GroupStruct>> *)pList;
    GroupStruct groupst;
    groupst.bo_caltype = 0;
    groupst.maximum_amounts = 0;
    groupst.minimum_amounts = 0;
    groupst.profit = 0;
    std::vector<uint32_t > cod(61, 0);
    groupst.condition.clear();

    uint32_t groupid = (uint32_t)atoi(pValue[0]);
    string symbols = pValue[1];
    groupst.bo_caltype = atoi(pValue[2]);
    int32_t maximum = atoi(pValue[3]);
    groupst.maximum_amounts = maximum * 1000;
    int32_t minimum = atoi(pValue[4]);
    groupst.minimum_amounts = minimum * 1000;
    groupst.profit = atoi(pValue[5]);

    char *buff = pValue[6];
    char *out_ptr = nullptr;
    int32_t i = 0;
    char *in_ptr[20];

    while ((in_ptr[i] = strtok_r(buff, ";", &out_ptr)) != nullptr){
        i++;
        buff = nullptr;
    }
    for(i = 0; i< 20 && in_ptr[i] != nullptr; i++)
        cod[atoi(in_ptr[i])] = 1;

    groupst.condition = cod;

    map<uint32_t ,map<string,GroupStruct>>::iterator itr;
    itr = pGroupSymbolsList->find(groupid);
    if (itr == pGroupSymbolsList->end()) {
        map<string,GroupStruct> data;
        data.insert( pair<string,GroupStruct>(symbols,groupst));
        pGroupSymbolsList->insert( pair<int32_t ,map<string,GroupStruct>>(groupid,data));
    } else {
        map<string,GroupStruct> &data = itr->second;
        data.insert( pair<string,GroupStruct>(symbols,groupst));
    }

    return 0;
}

//tradtype:0 == BO, other == FX
int32_t  UpdateGroupConsumer::GetGroupSpread(bool botrading,uint32_t groupid, const char *symbol, GroupStruct &groupstr){
    map<uint32_t ,map<string,GroupStruct >>::iterator itr;

    if(botrading){
        itr = m_pGroupBoSymbolsList.find(groupid);
        if (itr == m_pGroupBoSymbolsList.end())
        {
            return -1;
        }
    } else {
        itr = m_pGroupSymbolsList.find(groupid);
        if (itr == m_pGroupSymbolsList.end())
        {
            return -1;
        }
    }


    map<string,GroupStruct> &data = itr->second;
    map<string,GroupStruct>::iterator itr2;
    for (itr2=data.begin(); itr2!=data.end();)
    {
        string symbols = itr2->first;
        size_t found = symbols.find(symbol);
        if (found!=std::string::npos)
        {
            groupstr.bo_caltype =  itr2->second.bo_caltype;
            groupstr.maximum_amounts = itr2->second.maximum_amounts;
            groupstr.minimum_amounts = itr2->second.minimum_amounts;
            groupstr.profit = itr2->second.profit;
            groupstr.condition = itr2->second.condition;
            return 0;
        }
        ++itr2;
    }
    XERROR("The currency symbol was not found in %d group:%s\n",groupid,symbol);
    return -1;

}

int UpdateGroupConsumer::ReadGroupSymbolsList(bool botrading){

    sqlite3* dbhandle = nullptr;

    if(sqlite3_open("../configs/TE_group_security.db",&dbhandle) != SQLITE_OK){
        XERROR("Cannot open databse: %s", sqlite3_errmsg(dbhandle));
        return -1;
    }
    char* errmsg = nullptr;
    char sSQL[256] = {};
    int32_t ret = 0;

    if (botrading) {
        m_pGroupBoSymbolsList.clear();
        strcpy(sSQL, "select a.group_id,b.symbol_list,a.bo_caltype,a.maximum_amounts,a.minimum_amounts,a.profit,a.time_period from group_bo A,security_bo B where a.security_id = b.security_id;");
        ret =sqlite3_exec(dbhandle,sSQL,ReadGroupSymbolsCallback,&m_pGroupBoSymbolsList,&errmsg) ;
    } else {
        m_pGroupSymbolsList.clear();
        strcpy(sSQL, "SELECT A.group_id,B.symbol_list,A.security_spread,a.maximum_amounts,a.minimum_amounts,a.profit,a.time_period FROM group_fx A, security_fx B WHERE A.security_id = B.security_id;");
        ret = sqlite3_exec(dbhandle,sSQL,ReadGroupSymbolsCallback,&m_pGroupSymbolsList,&errmsg);
    }

    if (ret != SQLITE_OK){
        XERROR("sqlite3_exec error:sql[%s],error:%s\n", sSQL,errmsg);
        sqlite3_close(dbhandle);
        sqlite3_free(errmsg);
        dbhandle = nullptr;
        errmsg = nullptr;
        return -1;
    }

    sqlite3_close(dbhandle);
    dbhandle = nullptr;
    return 0;
}

/*
 * 1. ask=bid, bid=bid)
 * 2. ask=ask, bid=ask)
 * 3. ask=(ask+bid)/2, bid=(ask+bid)/2
 */
int UpdateGroupConsumer::CalAskBidSpread(bool botrading, int &ask, int &bid, int caltype){
    if(botrading) {
        switch (caltype) {
            case 1:
                ask = bid;
                break;
            case 2:
                bid = ask;
                break;
            case 3:
                bid = (ask + bid) >> 1;
                ask = bid;
                break;
            default:
                ask = bid;
                break;
        }
    } else {
        int a = caltype >> 1;
        int b = caltype - a;
        int value = (ask+bid) >> 1;
        int value2 = (ask+bid) - value ;
        ask = value + a ;
        bid = value2 - b ;
    }

    return 0;
}

void UpdateGroupConsumer::ShowGroupSymbolsList(bool botrading){
    map<unsigned int ,map<string,GroupStruct>>::iterator itr;
    map<unsigned int ,map<string,GroupStruct>> *pList = NULL;

    if(botrading){
        pList = &m_pGroupBoSymbolsList;
    } else {
        pList = &m_pGroupSymbolsList;
    }

    for(itr=pList->begin();itr!=pList->end();itr++){
        map<string,GroupStruct> &data = itr->second;
        for (map<string,GroupStruct>::iterator itr2=data.begin(); itr2!=data.end(); ++itr2){
            printf("Show Group:%d;symbols:%s;Bocaltype:%d;maximum:%d;minimum:%d;profit:%d\n",itr->first, (itr2->first).c_str(), itr2->second.bo_caltype,
                  itr2->second.maximum_amounts,itr2->second.minimum_amounts,itr2->second.profit);
        }
    }
}

UpdateGroupConsumer::UpdateGroupConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel) : channel_(channel){
    instance_id_ = instance_id;
}

UpdateGroupConsumer::~UpdateGroupConsumer(){

}

std::string UpdateGroupConsumer::GetEventName() {
    return instance_id_;
}

uint32_t UpdateGroupConsumer::EventHandle(u_int16_t tag, char *event, uint32_t eventlen){
    assert(event != nullptr);

    XINFO("Received UPDATEGROUP Msg: %s\n", event);
    do {
        rapidjson::Document doc;
        doc.Parse((const char *) event);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("json format error, parse code %d", code);
            break;
        }

        rapidjson::Value::ConstMemberIterator it;

        if ((it = doc.FindMember("group_boinfo_changed")) != doc.MemberEnd()) {
            ReadGroupSymbolsList(true);
            ShowGroupSymbolsList(true);
        }else if((it = doc.FindMember("group_security_changed")) != doc.MemberEnd() || (it = doc.FindMember("symbol_spread_changed")) != doc.MemberEnd()){
            ReadGroupSymbolsList(false);
            ShowGroupSymbolsList(false);
        }else {
            XINFO("There have no changed security_spread,group_spread, and group_bo_symbols");
        }

        return 0;

    } while (false);

    return 1;
}

uint32_t UpdateGroupConsumer::ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send){
    XERROR("\nERRNO %d, No implement\n", UNIVERSAL_TRADING_NO_IMPLEMENT_ERR);
    return UNIVERSAL_TRADING_NO_IMPLEMENT_ERR;
}


