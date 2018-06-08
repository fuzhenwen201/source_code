#include "BitCoinJsonRPC.h"
#include "HttpClient.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

size_t OnBitWriteData_Post(void* buffer, size_t size, size_t nmemb, void* lpVoid){
	auto str = static_cast<string*>(lpVoid);
	if (nullptr == str || nullptr == buffer){
		return 0;
	}

	auto pData = static_cast<char*>(buffer);
	str->append(pData, size * nmemb);

	return nmemb;
} 

BitCoinAPI::BitCoinAPI(const char* URL,const char* Name,const char* Pwd):URL_(URL),UserName_(Name),Pwd_(Pwd){

}

BitCoinAPI::~BitCoinAPI() = default;

bool BitCoinAPI::GetNewAddress(STExchangeAddrInfo* info){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};
    sprintf(data, "{\"jsonrpc\":\"1.0\",\"method\":\"getnewaddress\",\"params\":[\"%s\"],\"id\":1000}",info->sBTCLabel);

    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
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
        strcpy(info->sUserExchangeAddr, it->value.GetString());

        result = true;

    }while(false);

    return result;
}

std::string BitCoinAPI::GetAddressInfo(const char *pAddr){
	std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json");
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"getaccount\",\"params\":[\"%s\"],\"id\":1000}", pAddr);
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd,"%s:%s",UserName_.c_str(),Pwd_.c_str());
    client.SetUserInfo(user_pwd);
    // callback function
    client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
    // set culr
    client.SettingCURL();
    // Run
    client.Run();
    XINFO("result = %s",strResponse.c_str());

    std::string account = {};
    do{
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
        account = it->value.GetString();

    }while(false);

    return account;
}

bool BitCoinAPI::GetAddressesbyAccount(STExchangeAddrInfo* info){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"getaddressesbyaccount\",\"params\":[\"%s\"],\"id\":1000}",info->sBTCLabel);
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
	// set culr 
	client.SettingCURL();
	// Run 
	client.Run();

    do{
        rapidjson::Document doc;
        size_t pos = strResponse.find('{');
        doc.Parse(strResponse.c_str() + pos);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("parser json error = %d|event=%s", code, strResponse.c_str());
            break;
        }

        rapidjson::Value::ConstMemberIterator it;
        if ((it = doc.FindMember("result")) == doc.MemberEnd() || !it->value.IsArray()) {
            XERROR("There are no result members in the message!");
            break;
        }
        auto address_array = it->value.GetArray();
        for (uint32_t i = 0; i < address_array.Size(); ++i) {
           std::string address =  address_array[i].GetString();
        }

        return true;

    }while(false);

    return false;
}

double BitCoinAPI::GetBalance(STExchangeAddrInfo* info){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"getbalance\",\"params\":[\"%s\"],\"id\":1000}", info->sBTCLabel);
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
	// set culr 
	client.SettingCURL();
	// Run 
	client.Run();

    double balance = 0.0;
    do{
        rapidjson::Document doc;
        size_t pos = strResponse.find('{');
        doc.Parse(strResponse.c_str() + pos);
        if (doc.HasParseError() || !doc.IsObject()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            XERROR("parser json error = %d|event=%s", code, strResponse.c_str());
            break;
        }

        rapidjson::Value::ConstMemberIterator it;
        if ((it = doc.FindMember("result")) == doc.MemberEnd() || !it->value.IsDouble()) {
            XERROR("There are no result members in the message!");
            break;
        }
        balance = it->value.GetDouble();

    }while(false);

    return balance;
}

std::string BitCoinAPI::ShowPrivatekey(STExchangeAddrInfo* info){

    UnLockWallet(info->sPriKey, 100);

    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"dumpprivkey\",\"params\":[\"%s\"],\"id\":1000}",info->sUserExchangeAddr);
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd,"%s:%s",UserName_.c_str(),Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
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

    return result;
}

bool BitCoinAPI::LockWallet(){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"walletlock\",\"params\":[],\"id\":1000}");
    XINFO("data:%s", data);
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
	// set culr 
	client.SettingCURL();
	// Run 
	client.Run();

    return true;
}

bool BitCoinAPI::UnLockWallet(const char* passphrase,size_t timeout){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"walletpassphrase\",\"params\":[\"%s\",%lu],\"id\":1000}",passphrase,timeout);
    XINFO("data:%s", data);
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
	// set culr 
	client.SettingCURL();
	// Run 
	client.Run();

    return true;
}

bool BitCoinAPI::EncryptWallet(const char* passphrase){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"encryptwallet\",\"params\":[\"%s\"],\"id\":1000}",passphrase);
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
    client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
	// set culr 
	client.SettingCURL();
	// Run 
	client.Run();

    return true;
}

bool BitCoinAPI::SendToAddress(STTradeData* pData){

    this->UnLockWallet(pData->sPriKey, 100);
    
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"sendtoaddress\",\"params\":[\"%s\",\"%.9f\",\"0\",\"0\",true],\"id\":1000}",pData->sDestAddr,pData->fAmount);
    XINFO("data:%s", data);
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
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
        strcpy(pData->sTxID, it->value.GetString());
        pData->isSucess = 1;
        result = true;

    }while(false);

    this->LockWallet();

    return result;
}

bool BitCoinAPI::GetTransaction(STConfirmData* pData){

    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"gettransaction\",\"params\":[\"%s\"],\"id\":1000}",pData->sTxId);
    XINFO("data:%s", data);
    client.Post(URL_.c_str(), data, strlen(data));

    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
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
        if ((it = jvObject.FindMember("confirmations")) == doc.MemberEnd() || !it->value.IsInt()) {
            XERROR("There are no confirmations members or it's not array type in the message!");
            break;
        }
        pData->nConfirmBlkQty = static_cast<uint64_t>(it->value.GetInt());
        result = true;

    }while(false);

    return  result;
}

int32_t BitCoinAPI::GetBlockNum(){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"getblockcount\",\"params\":[],\"id\":1000}");
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
	// set culr 
	client.SettingCURL();
	// Run 
	client.Run();

    int32_t result = 0;
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
        if ((it = doc.FindMember("result")) == doc.MemberEnd() || !it->value.IsInt()) {
            XERROR("There are no result members in the message!");
            break;
        }

        result = it->value.GetInt();

    }while(false);

    return result;
}

bool BitCoinAPI::Listunspent(BlockChainInfo& info,std::map<std::string,STDepositAddr> *pDesportAddrMap,list<STDepositAddr>& desportInfo_out){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};

    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"listunspent\",\"params\":[0,3],\"id\":1000}");
    XINFO("data:%s",data);
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
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
        if ((it = doc.FindMember("result")) == doc.MemberEnd() || !it->value.IsArray()) {
            XERROR("There are no result members or it's not array type in the message!");
            break;
        }
        auto valArray = it->value.GetArray();

        for(rapidjson::SizeType i = 0; i < valArray.Size(); ++i){
            rapidjson::Value::ConstMemberIterator ita;
            if ((ita = valArray[i].FindMember("address")) == valArray[i].MemberEnd() || !ita->value.IsString()) {
                XERROR("There are no address members or it's not string type in the message!");
                continue;
            }
            auto destaddr = ita->value.GetString();
            auto tmp = pDesportAddrMap->find(destaddr);
            if(tmp == pDesportAddrMap->end()){
                continue;
            }
            auto item = tmp->second;

            if ((ita = valArray[i].FindMember("txid")) == valArray[i].MemberEnd() || !ita->value.IsString()) {
                XERROR("There are no txid members or it's not string type in the message!");
                continue;
            }
            strncpy(item.sTxID, ita->value.GetString(), TXIDLEN - 1);

            if ((ita = valArray[i].FindMember("amount")) == valArray[i].MemberEnd() || !ita->value.IsDouble()) {
                XERROR("There are no amount members or it's not string type in the message!");
                continue;
            }
            item.fAmount = ita->value.GetDouble();

            desportInfo_out.push_back(item);
        }

        result = true;

    }while(false);

    return result;
}

bool BitCoinAPI::GetBlockTemplate()
{
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json"); 
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"getblocktemplate\",\"params\":[],\"id\":1000}" );
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
	// callback function 
	client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post,&strResponse);
	// set culr 
	client.SettingCURL();
	// Run 
	client.Run();
    return true;
}

bool BitCoinAPI::GetBlockChainInfo()
{
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json");
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"getinfo\",\"params\":[],\"id\":1000}" );
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
    // callback function
    client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
    // set culr
    client.SettingCURL();
    // Run
    client.Run();

    return true;
}

bool BitCoinAPI::SetPayFee(double Amout){
    std::string strResponse;
    HttpClient client;
    client.Create();
    client.SetHeaders("Content-Type: application/json");
    char data[500] = {};
    sprintf(data,"{\"jsonrpc\":\"1.0\",\"method\":\"settxfee\",\"params\":[%.9f],\"id\":1000}",Amout);
    client.Post(URL_.c_str(), data, strlen(data));
    char user_pwd[50] = {};
    sprintf(user_pwd, "%s:%s", UserName_.c_str(), Pwd_.c_str());
    client.SetUserInfo(user_pwd);
    // callback function
    client.SettingBuffer(nullptr, nullptr, OnBitWriteData_Post, &strResponse);
    // set culr
    client.SettingCURL();
    // Run
    client.Run();

    bool result = false;
    do {
        rapidjson::Document doc;
        doc.Parse(strResponse.c_str());
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

