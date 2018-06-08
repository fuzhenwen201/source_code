#include "../include/onlineservice.h"

class CRedis rds;
char *g_RdsPort;
char *g_RdsIp;
char *g_RdsTimeout;
char *g_RdsPassword;
char *g_RdsIns;
int g_multiple_value ;

//std::vector<OnlineUsersInfo *> g_companyvector;
std::vector<string> g_companyvector;
std::vector<int> g_companysize;

int SetVirOnlineUsers(int company_num,int total_num,int &vir_num)
{
#if 0
    srand(unsigned(time(0)));
    if(company_num == 0)
    {
        vir_num = rand() % 50 + 100;
    }
    else if(company_num < 100 && company_num >= 1)
    {
         vir_num = rand() % 100 + company_num*100;

    }
    else if(company_num >= 100 && company_num <= 500 )
    {   
        vir_num = rand() % 100 +  100*company_num-(1/10)*company_num*company_num+1000;
    }
    else if (company_num > 500 && company_num <= 10000)
    {
        vir_num = rand() % 100 + 6*company_num-(3*company_num*company_num)/10000 +28000;
    }
    else
    {
        vir_num = company_num;
    }
#endif 
    //int speed_rate =  total_num / company_num;
    int real_num = company_num + total_num;
    srand(unsigned(time(0)));
    if(real_num == 0)
    {
        vir_num = rand() % 50 + 100;
    }
    else if(real_num < 100 && real_num >= 1)
    {
	//100-10000
         vir_num = rand() % 100 + (real_num)*100;
    }
    else if(real_num >= 100 && real_num <= 1000 )
    {   
	//10000-20000 抛物线
	//vir_num = rand() % 100 +  100*real_num + 1000;
	vir_num = int(-8.0/90*real_num*real_num)+ int(980*real_num/9.0)+ rand() % 100;
    }
    else if (real_num > 1000 && real_num <= 10000)
    {
	//20000-30000 抛物线
	vir_num = int(-1.0/81*(real_num-1000)*(real_num-1000))+20000;
    }
    else
    {
        vir_num = real_num+20000;
    }
    //vir_num = company_num > 0 ? g_multiple_value : (g_multiple_value*company_num);
    LOG_DEBUG("company_num [%d],vir_num : [%d]", company_num, vir_num);
    return 0 ;
}

int GetOnlineUser(CRedis *p_rds)
{
    // get total online users
    std::string key = "*@*memdb*";
    int onlinenum = p_rds->getkeyssize(key);
    std::string temp_minonline = p_rds->get("min_total_online");
    std::string temp_maxonline = p_rds->get("max_total_online");
    int minonline = atoi(temp_minonline.c_str());
    int maxonline = atoi(temp_maxonline.c_str());
    //std::string temp_onlinenum = std::to_string(onlinenum);
    char temp_onlinenum[10] = { 0 };
    sprintf(temp_onlinenum, "%d", onlinenum);
    std::string stronlinenum = temp_onlinenum;
    if(minonline > onlinenum)
        p_rds->set("min_total_online", stronlinenum);
    if(maxonline < onlinenum)
        p_rds->set("max_total_online", stronlinenum);
    p_rds->set("total_online", stronlinenum);
    // get company id from redis
    std::string company_str = p_rds->get("CRMAPICONFIG");

    Json::Reader reader;
    Json::Value value;
    Json::Value company_value;
    //LOG_INFO("company info :%s", company_str.c_str());
    if(reader.parse(company_str, value))
    {
        const Json::Value crmapi_value = value["crmapi"];

        for(size_t i = 0 ; i < crmapi_value.size(); i++)
        {
            if(crmapi_value[i].isMember("company"))
            {
                std::string company_id = crmapi_value[i]["company"].asString();
                std::string keys = std::string("*") + company_id + "*@*memdb*";
                int company_online = p_rds->getkeyssize(keys);
                std::string maxonline_key = company_id + "maxonline";
                std::string temp_maxonline = p_rds->get(maxonline_key);
                std::string minonline_key = company_id + "minonline";
                std::string temp_minonline = p_rds->get(minonline_key);
                std::string vironline_key = company_id + "vironline";
                //std::string temp_vironline = p_rds->get(vironline_key);
                int minonline = atoi(temp_minonline.c_str());
                int maxonline = atoi(temp_maxonline.c_str());
                int vironline =  -1;
                char temp_onlinenum[10] = { 0 };
                sprintf(temp_onlinenum, "%d", company_online);
                std::string stronlinenum = temp_onlinenum;
                if(minonline > company_online)
                    p_rds->set(minonline_key, stronlinenum);
                if(maxonline < company_online)
                    p_rds->set(maxonline_key, stronlinenum);
                std::string online_key = company_id + "online";
                p_rds->set(online_key, stronlinenum);
                SetVirOnlineUsers(company_online,onlinenum,vironline);
                char temp_vironline[10] = { 0 };
                sprintf(temp_vironline,"%d",vironline);
                p_rds->set(vironline_key,std::string(temp_vironline));
                //g_companyvector.push_back(company_id);
            }
        }
    }
    
    //get onlineusers according to company id

}
int Statisticsonlinenumber(CRedis *p_rds)
{
    // get company id from redis
    std::string company_str = p_rds->get("CRMAPICONFIG");

    Json::Reader reader;
    Json::Value value;
    Json::Value company_value;
    //LOG_INFO("company info :%s", company_str.c_str());
    if(reader.parse(company_str, value))
    {
        const Json::Value crmapi_value = value["crmapi"];
        //LOG_INFO("company num[%d]", crmapi_value.size());

        for(size_t i = 0 ; i < crmapi_value.size(); i++)
        {
            if(crmapi_value[i].isMember("company"))
            {
                std::string company_id = crmapi_value[i]["company"].asString();
                g_companyvector.push_back(company_id);
                //OnlineUsersInfo *pUser = new OnlineUsersInfo;
                //pUser->m_company_id = crmapi_value[i]["company"].asString();
                // g_companyvector.push_back(pUser);
                //LOG_INFO("company_id :%s", g_companyvector[i]->m_company_id.c_str());
            }


        }

    }
    // get online users
    std::string key = "*@*";
    //Json::Value value;
    value.clear();
    Json::Value item;
    Json::Value arrayobj;
    Json::Value company_info;
    Json::Reader read;
    Json::Value root;

    std::string curr_company_users ;
    std::string max_company_users  ;
    std::string min_company_users  ;
    std::string vir_company_users  ;

    int total_num = p_rds->getkeyssize(key);
    int online_time = time(NULL);
    value["curr_total_onlineuser"] = total_num;

    std::string str_online = p_rds->get(ONLINE_USERS);

    if(!str_online.empty())
    {
        if(read.parse(str_online, value))
        {
            if(total_num < value["min_total_onlineuser"].asInt())
            {
                root["min_total_onlineuser"] = total_num;
                root["min_time"] = online_time;
            }
            else
            {
                root["min_total_onlineuser"] = value["min_total_onlineuser"].asInt();
                root["min_time"] = value["min_time"].asInt();
            }

            if(total_num > value["max_total_onlineuser"].asInt())
            {
                root["max_total_onlineuser"] = total_num;
                root["max_time"] = online_time;
            }
            else
            {
                root["max_total_onlineuser"] = value["max_total_onlineuser"].asInt();
                root["max_time"] = value["max_time"].asInt();
            }

            const Json::Value read_company = value[ONLINE_COMPANY];
            size_t size = read_company.size();
            if(g_companyvector.size() < read_company.size())
                size = g_companyvector.size();

            for(size_t i = 0; i < size ; i++)
            {
                company_info.clear();
                //company_info["company_id"] = g_companyvector[i]->m_company_id;
                company_info["company_id"] = g_companyvector[i];
                curr_company_users = "current_onlineusers";
                max_company_users = "max_onlineusers";
                min_company_users = "min_onlineusers" ;
                vir_company_users = "vir_onlineusers" ;
                //std::string company_key = g_companyvector[i]->m_company_id + "*@*";
                std::string company_key = g_companyvector[i] + "*@*";
                int company_num =  p_rds->getkeyssize(company_key);

                //curr online users
                company_info[curr_company_users] = company_num ;
                //min online users

                if(read_company[i].isMember(min_company_users))
                {
                    if(company_num < read_company[i][min_company_users].asInt())
                    {
                        company_info[min_company_users] =  company_num;
                        company_info["min_time"] = online_time;
                    }
                    else
                    {
                        company_info[min_company_users] = read_company[i][min_company_users].asInt();
                        company_info["min_time"] = read_company[i]["min_time"].asInt();
                    }
                }
                else
                {
                    company_info[min_company_users] = company_num;
                    company_info["min_time"] = online_time;
                }
                // max online users
                if(read_company[i].isMember(max_company_users))
                {
                    if(company_num > read_company[i][max_company_users].asInt())
                    {
                        company_info[max_company_users] = company_num;
                        company_info["max_time"] = online_time;
                    }
                    else
                    {
                        company_info[max_company_users] = read_company[i][max_company_users].asInt();
                        company_info["max_time"] = read_company[i]["max_time"].asInt();
                    }
                }
                else
                {
                    company_info[max_company_users] = company_num;
                    company_info["max_time"] = online_time;
                }
                // vir online users
                int vir_onlineusers = 0;
               // SetVirOnlineUsers(company_num, vir_onlineusers);
                company_info[vir_company_users] = vir_onlineusers;
                item.append(company_info);

            }
            // 新增加了白标
            if(g_companyvector.size() > read_company.size())
            {
                size_t i = read_company.size();
                for(; i < g_companyvector.size(); i++)
                {
                    company_info.clear();
                    //company_info["company_id"] = g_companyvector[i]->m_company_id;
                    company_info["company_id"] = g_companyvector[i];
                    curr_company_users = "current_onlineusers";
                    max_company_users = "max_onlineusers";
                    min_company_users = "min_onlineusers" ;
                    vir_company_users = "vir_onlineusers" ;
                    //std::string company_key = g_companyvector[i]->m_company_id + "*@*";
                    std::string company_key = g_companyvector[i]  + "*@*";
                    int company_num =  p_rds->getkeyssize(company_key);
                    company_info[curr_company_users] = company_num ;
                    company_info[min_company_users] = company_num;
                    company_info["min_time"] = online_time;
                    company_info[max_company_users] = company_num;
                    company_info["max_time"] = online_time;
                    int vir_num = 0;
                    //SetVirOnlineUsers(company_num, vir_num);
                    company_info[vir_company_users] = vir_num;
                    item.append(company_info);
                }
            }

        }

    }
    else
    {


        root["min_total_onlineuser"] = total_num;
        root["min_time"] = online_time;
        root["max_total_onlineuser"] = total_num;
        root["max_time"] = online_time;

        for(size_t i = 0 ; i < g_companyvector.size(); i++)
        {
            company_info.clear();
            //company_info["company_id"] = g_companyvector[i]->m_company_id;
            company_info["company_id"] = g_companyvector[i];
            curr_company_users = "current_onlineusers";
            max_company_users = "max_onlineusers";
            min_company_users = "min_onlineusers" ;
            vir_company_users = "vir_onlineusers" ;
            //std::string company_key = g_companyvector[i]->m_company_id + "*@*";
            std::string company_key = g_companyvector[i] + "*@*";
            int company_num =  p_rds->getkeyssize(company_key);
            company_info[curr_company_users] = company_num ;
            company_info[min_company_users] = company_num;
            company_info["min_time"] = online_time;
            company_info[max_company_users] = company_num;
            company_info["max_time"] = online_time;
            int vir_num = 0;
            //SetVirOnlineUsers(company_num, vir_num);
            company_info[vir_company_users] = vir_num;
            item.append(company_info);
        }
    }
    //arrayobj.append(item);
    root[ONLINE_COMPANY] = item;
    //std::string data  = value.toStyledString();
    Json::FastWriter writer ;
    std::string data = writer.write(root);
    LOG_DEBUG("online: [%s]", data.c_str());
    p_rds->set(ONLINE_USERS, data);

    g_companyvector.clear();

    return 0;
}
void ApproachesVirOnline(CRedis* p_rds)
{
    // get company id from redis
    std::string company_str = p_rds->get("CRMAPICONFIG");

    Json::Reader reader;
    Json::Value value;
    Json::Value company_value;
    //LOG_INFO("company info :%s", company_str.c_str());
    if(reader.parse(company_str, value))
    {
        const Json::Value crmapi_value = value["crmapi"];

        for(size_t i = 0 ; i < crmapi_value.size(); i++)
        {
            if(crmapi_value[i].isMember("company"))
            {
                std::string company_id = crmapi_value[i]["company"].asString();
                std::string vironline_key = company_id + "vironline";
                std::string curronline_key = company_id + "currentonline";
                std::string temp_vironline = p_rds->get(vironline_key);
                std::string temp_curronline = p_rds->get(curronline_key);
                int vironline =  atoi(temp_vironline.c_str());
                int curronline = atoi(temp_curronline.c_str());
                int addonline =  SECINTERVALS*(vironline - curronline)/INTERVALS + curronline;
                char str_curronline[10] = { 0 };
                sprintf(str_curronline,"%d",addonline);
                LOG_DEBUG("company_id [%s]currentonline [%s]",company_id.c_str(),str_curronline);
                p_rds->set(curronline_key,std::string(str_curronline));
            }
        }
    }
}
void DetectionTimeOut(const boost::system::error_code &e , boost::asio::deadline_timer *t, void *param)
{
    CRedis *p_rds = (CRedis *)param;
    GetOnlineUser(p_rds);
    t->expires_at(t->expires_at() + boost::posix_time::seconds(INTERVALS));
    t->async_wait(boost::bind(DetectionTimeOut, boost::asio::placeholders::error, t, param));

}
void SettingShowOnline(const boost::system::error_code &e , boost::asio::deadline_timer *t, void *param)
{
    CRedis *p_rds = (CRedis *)param;
    ApproachesVirOnline(p_rds);
    
    t->expires_at(t->expires_at() + boost::posix_time::seconds(SECINTERVALS));
    t->async_wait(boost::bind(SettingShowOnline, boost::asio::placeholders::error, t, param));
}
int main(int argc, char *argv[])
{
    if(argc < 2)
        return FAILE;


    if( FAILE == InitLog("onlineservice", argv[1]))
    {
        return FAILE;
    }
    //init log

    //connect rds
    M_XRETURN(((g_RdsIp = getenv("RDS_IP")) == NULL),
              false, LOG_ERR("get env RDS_IP err!"));

    M_XRETURN(((g_RdsPort = getenv("RDS_PORT")) == NULL),
              false, LOG_ERR("get env RDS_PORT err!"));

    M_XRETURN(((g_RdsTimeout = getenv("RDS_CON_TIMEOUT")) == NULL),
              false, LOG_ERR("get env RDS_CON_TIMEOUT err!"));

    //M_XRETURN(((g_RdsPassword = getenv("RDS_PWD")) == NULL),
              //false, LOG_ERR("get env RDS_PWD err!"));
	g_RdsPassword = getenv("RDS_PWD");
    g_RdsIns = getenv("RDS_INS");
    //char *multiple = NULL;
    //M_XRETURN(((multiple = getenv("MULTIPLE")) == NULL),
              //false, LOG_ERR("get env MULTIPLE err!"));
    //g_multiple_value = atoi(multiple);
    LOG_INFO("init redis ip[%s],por[%s],time[%s],pwd[%s],ins[%s]", g_RdsIp, g_RdsPort, g_RdsTimeout, g_RdsPassword, g_RdsIns);
    int nRet ;
    if(nRet = rds.connect(g_RdsIp, atoi(g_RdsPort), atoi(g_RdsTimeout), g_RdsPassword == NULL ? "" : g_RdsPassword, g_RdsIns == NULL ? "" : g_RdsIns))
    {
        LOG_ERR("connect reids err[%d]", nRet);
        return FAILE;
    }
    // get company id from redis
#if 0
    std::string company_str = rds.get("CRMAPICONFIG");

    Json::Reader reader;
    Json::Value value;
    Json::Value company_value;
    LOG_INFO("company info :%s", company_str.c_str());
    if(reader.parse(company_str, value))
    {
        const Json::Value crmapi_value = value["crmapi"];
        LOG_INFO("company num[%d]", crmapi_value.size());

        for(size_t i = 0 ; i < crmapi_value.size(); i++)
        {
            if(crmapi_value[i].isMember("company"))
            {
                std::string company_id = crmapi_value[i]["company"].asString();
                g_companyvector.push_back(company_id);
                //OnlineUsersInfo *pUser = new OnlineUsersInfo;
                //pUser->m_company_id = crmapi_value[i]["company"].asString();
                // g_companyvector.push_back(pUser);
                //LOG_INFO("company_id :%s", g_companyvector[i]->m_company_id.c_str());
            }


        }

    }
#endif

    // boost timer
    boost::asio::io_service io;
    boost::asio::deadline_timer *timer = new  boost::asio::deadline_timer(io, boost::posix_time::seconds(INTERVALS));
    timer->async_wait(boost::bind(DetectionTimeOut, boost::asio::placeholders::error, timer, &rds));

    boost::asio::deadline_timer *sectimer = new boost::asio::deadline_timer(io, boost::posix_time::seconds(SECINTERVALS));
    sectimer->async_wait(boost::bind(SettingShowOnline, boost::asio::placeholders::error, sectimer, &rds));
    io.run();
    return  0;
}
