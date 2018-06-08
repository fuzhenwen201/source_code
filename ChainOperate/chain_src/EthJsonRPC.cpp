#include "EthJsonRPC.h"
#include "HttpClient.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <math.h>

int hex_char_value(char c){
    if(c >= '0' && c <= '9')
        return c - '0';
    else if(c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    else if(c >= 'A' && c <= 'F')
        return (c - 'A' + 10);

    return 0;
}

uint128_t hex2decimal(std::string& szHex){
    uint128_t number = 0;
    if(szHex[0] == '0' && (szHex[1] == 'x' || szHex[1] == 'X')){
        szHex= szHex.substr(2);
    }
    size_t size = szHex.size();
    for(uint32_t i = 0; i < size; i++) {
        uint128_t tmp = 1;
        for(int j = size - i - 1; j > 0; j--){
            tmp *= 16;
        }
        uint128_t tmp1 = tmp * hex_char_value(szHex[i]);
        number += tmp1;
    }
    return number;
}

const uint64_t ETH_TO_GWEI = 1000000000;
const double GWEI_TO_ETH = 1000000000.0;

double Hexstrwei2Doubleth(std::string &hexstr){
    uint128_t number = hex2decimal(hexstr);
    uint64_t u64val = number / ETH_TO_GWEI;
    return (u64val / GWEI_TO_ETH);
}

std::string Doubleth2hexStrwei(double number){
    uint64_t u64val = static_cast<uint64_t>(number * ETH_TO_GWEI);
    uint128_t u128val = u64val * ETH_TO_GWEI;
    return u128val.str(16);
}

size_t OnWriteData_Post(void* buffer, size_t size, size_t nmemb, void* lpVoid){
	auto str = reinterpret_cast<std::string*>(lpVoid);
	if (nullptr == str || nullptr == buffer)
	{
		return 0;
	}

	auto pData = reinterpret_cast<char*>(buffer);
	str->append(pData, size * nmemb);

	return nmemb;
}

ETHOperJsonRPC::ETHOperJsonRPC(const char* URL) : URL_(URL){

}

ETHOperJsonRPC::~ETHOperJsonRPC() = default;

bool ETHOperJsonRPC::NewAccount(STExchangeAddrInfo& account){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json");
    char data[500] = { 0 };
    sprintf(data,"{\"jsonrpc\":\"2.0\",\"method\":\"personal_newAccount\",\"params\":[\"%s\"],\"id\":1000}",account.sPriKey);
    client.Post(URL_.c_str(), data, strlen(data));
    XINFO("data:%s", data);
	// callback function 
	client.SettingBuffer(nullptr, nullptr,OnWriteData_Post,(void *)&strResponse);
	// set culr 
	client.SettingCURL();
	// Run 
	client.Run();

    bool result = false;
    do {
        rapidjson::Document doc;
        size_t pos = strResponse.find('{');
        doc.Parse(strResponse.c_str() + pos);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("parser json error = %d|event=%s", code, strResponse.c_str());
            break;
        }

        rapidjson::Value::ConstMemberIterator it;
        if ((it = doc.FindMember("result")) == doc.MemberEnd() || !it->value.IsString()) {
            XERROR("There are no result members in the message!");
            break;
        }
        strncpy(account.sUserExchangeAddr, it->value.GetString(), ADDRESSLEN);
        result = true;

    }while(false);

    return result;
}

bool ETHOperJsonRPC::UnlockAccount(STTradeData& tx){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json");
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"2.0\",\"method\":\"personal_unlockAccount\",\"params\":[\"%s\",\"%s\",30],\"id\":1000}",
                                                                      tx.sSrcAddr,tx.sPriKey);
    client.Post(URL_.c_str(), data, strlen(data));
    XINFO("data= %s", data);
    // callback function
    client.SettingBuffer(nullptr, nullptr, OnWriteData_Post, (void *)&strResponse);
    // set culr
    client.SettingCURL();
    // Run
    client.Run();

    bool result = false;
    do {
        rapidjson::Document doc;
        size_t pos = strResponse.find('{');
        doc.Parse(strResponse.c_str() + pos);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("parser json error = %d|event=%s", code, strResponse.c_str());
            break;
        }

        rapidjson::Value::ConstMemberIterator it;
        if ((it = doc.FindMember("result")) == doc.MemberEnd() || !it->value.IsBool()) {
            XERROR("There are no result members in the message!");
            break;
        }
        result = it->value.GetBool();

    }while(false);

    return result;
}

bool ETHOperJsonRPC::LockAccount(STTradeData& tx){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json");
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"2.0\",\"method\":\"personal_lockAccount\",\"params\":[\"%s\"],\"id\":1000}", tx.sSrcAddr);
    client.Post(URL_.c_str(), data, strlen(data));
    XINFO("data= %s", data);
    // callback function
    client.SettingBuffer(nullptr, nullptr, OnWriteData_Post, (void *)&strResponse);
    // set culr
    client.SettingCURL();
    // Run
    client.Run();

    bool result = false;
    do {
        rapidjson::Document doc;
        size_t pos = strResponse.find('{');
        doc.Parse(strResponse.c_str() + pos);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("parser json error = %d|event=%s", code, strResponse.c_str());
            break;
        }

        rapidjson::Value::ConstMemberIterator it;
        if ((it = doc.FindMember("result")) == doc.MemberEnd() || !it->value.IsBool()) {
            XERROR("There are no result members in the message!");
            break;
        }
        result = it->value.GetBool();

    }while(false);

    return result;
}

bool ETHOperJsonRPC::SendTransaction(STTradeData& tx,bool isFlag){
   
    if (!this->UnlockAccount(tx)) {
        XERROR("UnlockAccount fail when ETH SendTransaction");
        return false;
    }

    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json");

    std::string strAmount = {};
    std::string inputdata = {};
    auto gaslimit = static_cast<uint64_t>(tx.fGas * ETH_TO_GWEI * ETH_TO_GWEI / tx.nGasPrice);
    uint128_t u128gas = gaslimit;
    std::string strGas = "0x" + u128gas.str(16);
    uint128_t gasPrice = tx.nGasPrice;
    std::string strGasPrice = "0x" + gasPrice.str(16);
    char data[1000] = {};
    if(tx.uCoinId == 2) {
        if (tx.nDirect == 0x9003) {
            double remained = tx.fAmount - tx.fGas;
            strAmount = "0x" + Doubleth2hexStrwei(remained);
        } else {
            strAmount = "0x" + Doubleth2hexStrwei(tx.fAmount);
        }

        sprintf(data,"{\"jsonrpc\":\"2.0\",\"method\":\"eth_sendTransaction\",\"params\":[{\"from\":\"%s\",\"to\":\"%s\","
                        "\"gas\":\"%s\",\"gasPrice\":\"%s\",\"value\":\"%s\"}],\"id\":1000}",
                tx.sSrcAddr, tx.sDestAddr, strGas.c_str(), strGasPrice.c_str(), strAmount.c_str());
    }else{
        strAmount = "0x0";
        std::string DestAddr = tx.sDestAddr;
        auto denominator = static_cast<uint64_t>(pow(10, tx.uDecimals));
        uint64_t count = static_cast<uint64_t>(tx.fAmount * 1000000000);
        uint128_t amount = 0;
        if(tx.uDecimals >= 9){
            uint128_t aamount = denominator / 1000000000;
            amount = aamount * count;
        }else{
            auto tmpdenom = static_cast<uint64_t>(pow(10, 9 - tx.uDecimals));
            amount = count / tmpdenom;
        }
        std::string hexamount = amount.str(16, 64);

        inputdata = "0xa9059cbb000000000000000000000000" +  DestAddr.substr(2) + hexamount;

        sprintf(data,"{\"jsonrpc\":\"2.0\",\"method\":\"eth_sendTransaction\",\"params\":[{\"from\":\"%s\",\"to\":\"%s\","
                        "\"gas\":\"%s\",\"gasPrice\":\"%s\",\"value\":\"%s\",\"data\":\"%s\"}],\"id\":1000}",
                tx.sSrcAddr, tx.sContractAddr, strGas.c_str(), strGasPrice.c_str(), strAmount.c_str(),inputdata.c_str());
    }

    XINFO("data:%s",data);
    client.Post(URL_.c_str(), data, strlen(data));

	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnWriteData_Post, (void *)&strResponse);
	// set culr 
	client.SettingCURL();
	// Run 
	client.Run();

    bool result = false;
    do {
        rapidjson::Document doc;
        size_t pos = strResponse.find('{');
        doc.Parse(strResponse.c_str() + pos);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("parser json error = %d|event=%s", code, strResponse.c_str());
            break;
        }

        rapidjson::Value::ConstMemberIterator it;
        if ((it = doc.FindMember("result")) == doc.MemberEnd() || !it->value.IsString()) {
            XERROR("There are no result members in the message!");
            break;
        }
        strncpy(tx.sTxID, it->value.GetString(), TXIDLEN - 1);

        tx.isSucess = 1;
        result = true;

    }while(false);

    this->LockAccount(tx);

    return result;
}

std::string ETHOperJsonRPC::GetETHBalance(std::string& addr){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json");
    char data[1000] = {};

    sprintf(data,"{\"jsonrpc\":\"2.0\",\"method\":\"eth_getBalance\",\"params\":[\"%s\",\"latest\"],\"id\":1000}",addr.c_str());
    printf("data= %s\n", data);
    client.Post(URL_.c_str(), data, strlen(data));
    // callback function
    client.SettingBuffer(nullptr, nullptr,OnWriteData_Post,(void *)&strResponse);
    // set culr
    client.SettingCURL();
    // Run
    client.Run();

    std::string result = {};
    do {
        rapidjson::Document doc;
        size_t pos = strResponse.find('{');
        doc.Parse(strResponse.c_str() + pos);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("parser json error = %d|event=%s", code, strResponse.c_str());
            break;
        }

        rapidjson::Value::ConstMemberIterator it;
        if ((it = doc.FindMember("result")) == doc.MemberEnd() || !it->value.IsString()) {
            XERROR("There are no result members in the message!");
            break;
        }
        result = it->value.GetString();
    }while(false);

    printf("ETHBalance = %s",result.c_str());

    return result;
}

uint64_t ETHOperJsonRPC::GetETHBlockNumber() {
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json");
    char data[1000] = {};

    sprintf(data,"{\"jsonrpc\":\"2.0\",\"method\":\"eth_blockNumber\",\"params\":[],\"id\":1000}");
    XINFO("data= %s", data);
    client.Post(URL_.c_str(), data, strlen(data));
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnWriteData_Post, (void *)&strResponse);
	// set culr 
	client.SettingCURL();
	// Run 
	client.Run();

    uint64_t result = 0;
    do {
        rapidjson::Document doc;
        size_t pos = strResponse.find('{');
        doc.Parse(strResponse.c_str() + pos);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("parser json error = %d|event=%s", code, strResponse.c_str());
            break;
        }

        rapidjson::Value::ConstMemberIterator it;
        if ((it = doc.FindMember("result")) == doc.MemberEnd() || !it->value.IsString()) {
            XERROR("There are no result members in the message!");
            break;
        }
        std::string tmp = it->value.GetString();
        result = hex2decimal(tmp);

    }while(false);

    return result;
}

bool ETHOperJsonRPC::GetBlockbyNumber(uint32_t block,std::map<std::string,STDepositAddr> *pDesportAddrMap,list<STDepositAddr>& desportInfo_out){
    std::string strResponse = {};
    HttpClient client = {};
    client.Create();
    client.SetHeaders("Content-Type: application/json");
    char data[1000] = { 0 };

    sprintf(data,"{\"jsonrpc\":\"2.0\",\"method\":\"eth_getBlockByNumber\",\"params\":[\"0x%x\",true],\"id\":1000}", block);
    XINFO("data= %s", data);

    client.Post(URL_.c_str(), data, strlen(data));
    // callback function
    client.SettingBuffer(nullptr, nullptr, OnWriteData_Post, (void *)&strResponse);
    // set culr
    client.SettingCURL();
    // Run
    client.Run();

    bool result = false;
    do {
        rapidjson::Document doc;
        size_t pos = strResponse.find('{');
        doc.Parse(strResponse.c_str() + pos);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("parser json error = %d|event=%s", code, strResponse.c_str());
            break;
        }

        if ((!doc.HasMember("result"))) {
            XERROR("There are no result members in the message!");
            break;
        }
        const rapidjson::Value &jvObject = doc["result"];
        if (!jvObject.IsObject()) {
            XERROR("The result member is not Object type in the message!");
            break;
        }

        rapidjson::Value::ConstMemberIterator it;
        if ((it = jvObject.FindMember("transactions")) == jvObject.MemberEnd() || !it->value.IsArray()) {
            XERROR("There are no transactions members or it's not array type in the message!");
            break;
        }
        auto valArray = it->value.GetArray();

        for(rapidjson::SizeType i = 0; i < valArray.Size(); ++i){
            rapidjson::Value::ConstMemberIterator ita;

            STDepositAddr item = {};

            if ((ita = valArray[i].FindMember("value")) == valArray[i].MemberEnd() || !ita->value.IsString()) {
                XERROR("There are no hash members or it's not string type in the message!");
                continue;
            }
            std::string tmpvalue = ita->value.GetString();

            if("0x0" == tmpvalue){
                if ((ita = valArray[i].FindMember("input")) == valArray[i].MemberEnd() || !ita->value.IsString()) {
                    //XERROR("There are no to members or it's not string type in the message!");
                    continue;
                }
                std::string strinput = ita->value.GetString();

                if(strinput.substr(0,10) == "0xa9059cbb" && strinput.size() == 138) {
                    XINFO("input:%s",strinput.c_str());
                    auto destaddr2 = "0x" + strinput.substr(34, 40);
                    auto strtmp = strinput.substr(74, 64);
                    auto strpos = strtmp.find_first_not_of('0');
                    std::string destvalue = {};
                    if (strpos != std::string::npos) {
                        destvalue = "0x" + strtmp.substr(strpos);
                    }else{
                        continue;
                    }

                    auto tmp = pDesportAddrMap->find(destaddr2);
                    if(tmp == pDesportAddrMap->end()){
                        continue;
                    }else{
                        item = tmp->second;
                        strncpy(item.sDestAddr, destaddr2.c_str(), ADDRESSLEN - 1);
                        strncpy(item.sTokenValue, destvalue.c_str(), ADDRESSLEN - 1);
                        item.fAmount = 0.0;
                    }
                }else{
                    continue;
                }
            }

            if ((ita = valArray[i].FindMember("to")) == valArray[i].MemberEnd() || !ita->value.IsString()) {
                //XERROR("There are no to members or it's not string type in the message!");
                continue;
            }
            auto destaddr = ita->value.GetString();
            if("0x0" == tmpvalue){
                strncpy(item.sContractAddr, destaddr, ADDRESSLEN - 1);
            }else {
                auto tmp = pDesportAddrMap->find(destaddr);
                if (tmp == pDesportAddrMap->end()) {
                    continue;
                }
                item = tmp->second;
                strncpy(item.sDestAddr, destaddr, ADDRESSLEN - 1);
                item.fAmount = Hexstrwei2Doubleth(tmpvalue);
            }
            XINFO("destaddr:%s,destvalue:%s,contractaddr:%s", item.sDestAddr, item.sTokenValue,item.sContractAddr);

            if ((ita = valArray[i].FindMember("from")) == valArray[i].MemberEnd() || !ita->value.IsString()) {
                XERROR("There are no from members or it's not string type in the message!");
                continue;
            }
            strncpy(item.sSrcAddr, ita->value.GetString(), ADDRESSLEN - 1);

            if ((ita = valArray[i].FindMember("hash")) == valArray[i].MemberEnd() || !ita->value.IsString()) {
                XERROR("There are no hash members or it's not string type in the message!");
                continue;
            }
            strncpy(item.sTxID, ita->value.GetString(), TXIDLEN - 1);

            if ((ita = valArray[i].FindMember("blockNumber")) == valArray[i].MemberEnd() || !ita->value.IsString()) {
                XERROR("There are no hash members or it's not string type in the message!");
                continue;
            }
            std::string tmpnumber = ita->value.GetString();
            uint64_t blocknumber = hex2decimal(tmpnumber);
            item.nScanStartBlock = blocknumber;

            XINFO("**********************ETH Depost***************************\n"
                          "ETHOperJsonRPC find deposit: DstAddr:%s,aount:%f,txid:%s,blockNumber:%0x",
                  item.sDestAddr, item.fAmount, item.sTxID, item.nScanStartBlock);

            desportInfo_out.push_back(item);
            result = true;
        }

    }while(false);

    return result;
}

bool ETHOperJsonRPC::CheckingTransactionState(STConfirmData& tx){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json");
    char data[2000] = { 0 };
  
    sprintf(data,"{\"jsonrpc\":\"2.0\",\"method\":\"eth_getTransactionReceipt\",\"params\":[\"%s\"],\"id\":%d}", tx.sTxId,1000);
    XINFO("data= %s", data);
    client.Post(URL_.c_str(), data, strlen(data));
    // callback function
    client.SettingBuffer(nullptr, nullptr, OnWriteData_Post, (void *)&strResponse);
    // set curl
    client.SettingCURL();
    // Run
    client.Run();

    bool result = false;
    do {
        rapidjson::Document doc;
        size_t pos = strResponse.find('{');
        doc.Parse(strResponse.c_str() + pos);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("parser json error = %d|event=%s", code, strResponse.c_str());
            break;
        }

        if ((!doc.HasMember("result"))) {
            XERROR("There are no result members in the message!");
            break;
        }
        const rapidjson::Value &jvObject = doc["result"];
        if (!jvObject.IsObject()) {
            XERROR("The result member is not Object type in the message!");
            break;
        }

        rapidjson::Value::ConstMemberIterator it;
        if ((it = jvObject.FindMember("gasUsed")) == doc.MemberEnd() || !it->value.IsString()) {
            XERROR("There are no gasUsed members or it's not array type in the message!");
            break;
        }
        std::string strused = it->value.GetString();
        uint64_t valused = hex2decimal(strused);
        uint64_t usedGwei = valused * tx.nGasPrice;
        tx.nGasUsed = usedGwei / GWEI_TO_ETH;

        if ((it = jvObject.FindMember("blockNumber")) == doc.MemberEnd() || !it->value.IsString()) {
            XERROR("There are no blockNumber members or it's not array type in the message!");
            break;
        }
        std::string tmp = it->value.GetString();
        tx.nBlockNumber = hex2decimal(tmp);

        uint64_t blocknumber = GetETHBlockNumber();
        tx.nConfirmBlkQty = blocknumber - tx.nBlockNumber + 1;

        if ((it = jvObject.FindMember("status")) == doc.MemberEnd() || !it->value.IsString()) {
            XERROR("There are no status members or it's not array type in the message!");
            break;
        }
        std::string status = it->value.GetString();
        tx.status = hex2decimal(status);
        result = true;

    }while(false);

    return result;
 }

bool ETHOperJsonRPC::CheckTransaction(BlockChainInfo& info,std::map<std::string,STDepositAddr> *pDesportAddrMap,list<STDepositAddr>& desportInfo_out){

    auto currentHeight = GetETHBlockNumber();
	XINFO("ETHOperJsonRPC Begin scan at 0x%0x, total blocks:0x%0x",info.sPosNum + 1, currentHeight);
    bool result = true;
    for (auto i = info.sPosNum + 1; i <= currentHeight; i++)
    {
        if(!GetBlockbyNumber(i, pDesportAddrMap, desportInfo_out))
        {
            XINFO("There is no consistent data in block 0x%0x", i);
            result = false;
        }
        info.sPosNum = i;
    }

    return result;
 }

  