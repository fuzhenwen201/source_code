//
// Created by alex on 1/13/17.
//

#include <unistd.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <iostream>
#include <curl/curl.h>
#include "xlogger.h"
#include "marketdata.h"


using namespace snetwork_signalengine_marketdata;


MarketData::MarketData(){
    domain_ = "";
    accessToken_ = "";
    symbols_ =  {
		"AUDCAD",
		"AUDSGD",
		"CADSGD",
		"EURCAD",
		"GBPJPY",
		"NZDJPY",
		"SGDJPY",
		"USDJPY",
		"ZARJPY",
		"AUDCHF",
		"AUDUSD",
		"EURCHF",
		"GBPAUD",
		"GBPNZD",
		"TRYJPY",
		"USDSGD",
		"CADCHF",
		"CHFJPY",
		"EURJPY",
		"EURSGD",
		"GBPCAD",
		"NZDCAD",
		"NZDUSD",
		"USDCAD",
		"USDHKD",
		"AUDJPY",
		"EURTRY",
		"GBPCHF",
		"GBPSGD",
		"NZDCHF",
		"USDCHF",
		"USDTRY",
		"AUDNZD",
		"CADJPY",
		"EURAUD",
		"EURGBP",
		"EURNZD",
		"EURUSD",
		"GBPUSD",
		"USDCNH",
		"USDINR"

	};

    frequency_ = 100000;

    message_buffer_.buff = nullptr;
    message_buffer_.size = 0;
    message_buffer_.used = 0;

    run_flag_ = false;
    stop_flag_ = true;

    chunk_ = nullptr;
    curl_ = nullptr;
}


MarketData::~MarketData(){
    Stop();

    while (run_flag_){
        usleep(500);
    }

    tnode_.Stop();
    tnode_.UnBind(UNIVERSAL_MARKET);

    if (nullptr != curl_){
        curl_easy_cleanup(curl_);
    }

    if (nullptr != chunk_){
        curl_slist_free_all(chunk_);
    }

    if (nullptr != message_buffer_.buff){
        delete  message_buffer_.buff;
    }
}

static uint64_t GetTimestamp(const char *date){
    int32_t year,month,day,hour,min,sec,usec;

    assert(nullptr != date);

    sscanf(date, "%d-%d-%dT%d:%d:%d.%dZ", &year,&month,&day,&hour,&min,&sec,&usec);

    struct tm stm;
    memset(&stm,0,sizeof(stm));

    stm.tm_year = year - 1900;
    stm.tm_mon = month - 1;
    stm.tm_mday = day;
    stm.tm_hour = hour;
    stm.tm_min = min;
    stm.tm_sec = sec;

    return  ((uint64_t)mktime(&stm)) * 1000000 + usec;
}

static size_t ReadCallback(void *data, size_t size, size_t nmemb, MessageBuffer* buffer)
{
    uint32_t data_len = (uint32_t)(size * nmemb);

    assert(nullptr != buffer);

    if(!data || data_len < 1){
        XERROR("\nERRNO %d, received empty data\n", UNIVERSAL_MARKET_RECEIVED_EMPTY_DATA_ERR);
        return 0;
    }

    // 1 is '\0'
    if (buffer->size < (data_len + buffer->used + 1)){
        XERROR("\nERRNO %d, data is bigger than buffer\n", UNIVERSAL_MARKET_NO_ENOUGH_DATA_BUFFER_ERR);
        return 0;
    }

    memcpy(buffer->buff + buffer->used, data, data_len);
    buffer->used += data_len;

    return data_len;
}


uint32_t MarketData::Init(snetwork_xservice_tnode::TNodeConfig &cfg){
    std::string str;
    uint32_t symbol_size;

    symbol_size = (uint32_t)symbols_.size();

    if (SymbolBUTT != symbol_size){
        XERROR("\nERRNO %d, config parameter error\n", UNIVERSAL_MARKET_CONFIG_PARAMETER_ERR);
        return UNIVERSAL_MARKET_CONFIG_PARAMETER_ERR;
    }

    curl_ = curl_easy_init();
    if(!curl_){
        XERROR("\nERRNO %d, lib curl init failed\n", UNIVERSAL_MARKET_INIT_FAILED_ERR);
        return UNIVERSAL_MARKET_INIT_FAILED_ERR;
    }

    str = domain_ + "/v1/prices?instruments=";

    //can't include simulated currency FINCOIN_USD
    symbol_size = symbol_size - 1;

    for(uint32_t i = 0; i < symbol_size; ++i){
        str +=  symbols_[i];

        if (i != (symbol_size - 1)){
            str += "%2C";
        }
    }

    curl_easy_setopt(curl_, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(curl_, CURLOPT_TIMEOUT, 10 );
    curl_easy_setopt(curl_, CURLOPT_URL, str.c_str());
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, ReadCallback);

    str = "Authorization: Bearer " + accessToken_;

    chunk_ = curl_slist_append(chunk_, str.c_str());
    if (!chunk_){
        XERROR("\nERRNO %d, lib curl add authorization failed\n", UNIVERSAL_MARKET_INIT_FAILED_ERR);
        return UNIVERSAL_MARKET_INIT_FAILED_ERR;
    }

    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, chunk_);

    tnode_.Config(cfg);
    tnode_.Bind(UNIVERSAL_MARKET);
    tnode_.Run();

    message_buffer_.size = 1024 * 15;
    message_buffer_.buff = new char[1024 * 15];

    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &message_buffer_);

    return  UNIVERSAL_MARKET_SUCCESS;
}


uint32_t MarketData::Run(){
    CURLcode res;
    snetwork_xservice_tnode::TNodeChannel channel = tnode_.GetChannel();
    std::string event = "$";

    run_flag_ = true;
    stop_flag_ = false;

    while (!stop_flag_){

        message_buffer_.used = 0;

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl_);

        /* Check for errors */
        if (res != CURLE_OK) {
            XERROR("\nERRNO %d, lib curl get failed, libcurl code : %s\n", UNIVERSAL_MARKET_GET_MARKETDATA_ERR, curl_easy_strerror(res));
         //   PublishMarketDataForException(channel, event);
            usleep(frequency_);
            continue;
        }

        uint32_t retcode = 0;
        res = curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE , &retcode);
        if ( (res == CURLE_OK) && retcode == 200 ){
            if (0 == message_buffer_.used){
             //   PublishMarketDataForException(channel, event);
                usleep(frequency_);
                continue;
            }

            //Fill '\0'
            //补上结束符
            message_buffer_.buff[message_buffer_.used] = 0;

            PublishMarketDataForNormal(message_buffer_.buff, channel, event);
        } else {
            //PublishMarketDataForException(channel, event);
        }

        usleep(frequency_);
    }

    run_flag_ = false;

    return  UNIVERSAL_MARKET_SUCCESS;
}


uint32_t MarketData::Stop(){

    stop_flag_ = true;

    return  UNIVERSAL_MARKET_SUCCESS;
}


void MarketData::SetAccessToken(const std::string &token){
    accessToken_ = token;
}

void MarketData::SetDomain(const std::string &domain){
    domain_ = domain;
}

void MarketData::PublishMarketDataForNormal(char *buff, snetwork_xservice_tnode::TNodeChannel &channel, std::string &event){
    assert(nullptr != buff);

    rapidjson::Document doc;

    {
        int i = 0;
        int j = 0;
        char *str = buff;

        while (str[i]){
            if (str[i]=='\n' || str[i]=='\r' || str[i]=='\t' || str[i]==' '){
                ++i;
                continue;
            }

            if (str[i] != '\"'){
                str[j] = str[i];
            }else {
                str[j] = '"';
            }

            ++i;
            ++j;
        }

        str[j] = 0;
    }

    doc.Parse((const char*)buff);
    if (doc.HasParseError()) {
        rapidjson::ParseErrorCode code = doc.GetParseError();
        XERROR("\nERRNO %d, json format error, parse code %d\n", UNIVERSAL_MARKET_GET_MARKETDATA_ERR, code);
        return;
    }

    rapidjson::Value & prices = doc["prices"];
    CurrencySymbolStatus symbol_status;
    CurrencySymbol *data_buffer;

    if (prices.IsArray()) {
        uint32_t prices_size = prices.Size();

        if ((SymbolBUTT - 1) != prices_size){
            XERROR("\nERRNO %d, config parameter error\n", UNIVERSAL_MARKET_CONFIG_PARAMETER_ERR);
            return;
        }

        for (uint32_t i = 0; i < prices_size; ++i) {
            rapidjson::Value & v = prices[i];
            assert(v.IsObject());

            symbol_status = CURRENCY_SYMBOL_NORMAL;

            if (v.HasMember("status") && v["status"].IsString()){
                std::string status = v["status"].GetString();
                if ("halted" == status){
                    symbol_status = CURRENCY_SYMBOL_HALTED;
                }
            }

            data_buffer = (CurrencySymbol*)channel.AllocDataBuff(sizeof(CurrencySymbol));
            if (nullptr == data_buffer){
                XERROR("\nERRNO %d, alloc data buffer failed\n", UNIVERSAL_MARKET_ALLOC_DATA_BUFFER_ERR);
                continue;
            }

            if (v.HasMember("instrument") && v["instrument"].IsString()) {
                //Because the query sequence is set, the default currency pair corresponds to the enumeration value of MarketSymbol
                //由于查询顺序设置，默认货币对与枚举值 MarketSymbol 对应
                data_buffer->instrument = htobe32(i);
                std::string group = v["instrument"].GetString();

                if (v.HasMember("time") && v["time"].IsString()) {
                    data_buffer->timestamp = GetTimestamp(v["time"].GetString());
                    data_buffer->timestamp = htobe64(data_buffer->timestamp);

                    if (v.HasMember("bid")) {
                        //Convert to integer by 6 bit precision
                        //按6位精度转换为整数
                        if (v["bid"].IsFloat()){
                            data_buffer->bid = (int32_t) (v["bid"].GetFloat() * 1000000);
                        } else if ((v["bid"].IsInt())){
                            data_buffer->bid = (int32_t) (v["bid"].GetInt() * 1000000);
                        } else {
                            XERROR("\nERRNO %d, message format error\n", UNIVERSAL_MARKET_MARKETDATA_FIELD_ERR);
                            continue;
                        }

                        data_buffer->bid = htobe32((uint32_t)data_buffer->bid);

                        if (v.HasMember("ask")) {
                            //Convert to integer by 6 bit precision
                            //按6位精度转换为整数
                            if (v["ask"].IsFloat()){
                                data_buffer->ask = (int32_t) (v["ask"].GetFloat() * 1000000);
                            } else if (v["ask"].IsInt()){
                                data_buffer->ask = (int32_t) (v["ask"].GetInt() * 1000000);
                            } else {
                                XERROR("\nERRNO %d, message format error\n", UNIVERSAL_MARKET_MARKETDATA_FIELD_ERR);
                                continue;
                            }

                            data_buffer->ask = htobe32((uint32_t)data_buffer->ask);

                            data_buffer->status = htobe32(symbol_status);

                            channel.PublishGroup(group, event,(char*)data_buffer, sizeof(CurrencySymbol), 0);
                            continue;
                        }
                    }
                }
            }

            channel.FreeDataBuff((char*)data_buffer);
            XERROR("\nERRNO %d, message content error\n", UNIVERSAL_MARKET_MARKETDATA_FIELD_ERR);
        }
    }

}

void MarketData::PublishMarketDataForException(snetwork_xservice_tnode::TNodeChannel &channel, std::string &event){
    CurrencySymbolStatus symbol_status = CURRENCY_SYMBOL_EXCPTION;
    CurrencySymbol *data_buffer;

    for (uint32_t i = 0; i < (SymbolBUTT - 1); ++i) {
        data_buffer = (CurrencySymbol*)channel.AllocDataBuff(sizeof(CurrencySymbol));
        if (nullptr == data_buffer){
            XERROR("\nERRNO %d, alloc data buffer failed\n", UNIVERSAL_MARKET_ALLOC_DATA_BUFFER_ERR);
            continue;
        }

        std::string group = symbols_[i];
        data_buffer->instrument = htobe32(i);
        data_buffer->status = htobe32(symbol_status);
        data_buffer->bid = 0;
        data_buffer->ask = 0;
        data_buffer->timestamp = 0;

        channel.PublishGroup(group, event,(char*)data_buffer, sizeof(CurrencySymbol), 0);
    }
}
