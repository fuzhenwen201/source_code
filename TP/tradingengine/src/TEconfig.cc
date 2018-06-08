#include <iostream>
#include "TEconfig.h"
#include<gflags/gflags.h>

namespace snetwork_xservice_xflagger {
    DEFINE_bool(screen, false, "printf message on terminator");
    DEFINE_bool(color, true, "message color");
    DEFINE_string(loggerDir, "./log", "logger directory");
    DEFINE_string(loggerFile, "tradingengine", "logger file name");
    DEFINE_string(payRatio, "80", "payratio");
    DEFINE_string(Id, "1", "ID");
    DEFINE_string(mqvhost, "/demo", "rabbitmq vhost");
    DEFINE_string(mqExchange, "t_node", "rabbitmq exchange");
    DEFINE_string(mqExchangeGroup, "t_node_grp", "rabbitmq exchange group");
    DEFINE_string(mqAddress, "localhost", "rabbitmq ip address");
    DEFINE_string(mqPort, "5672", "rabbitmq port");
    DEFINE_string(mqUser, "demo", "rabbitmq user");
    DEFINE_string(mqPassword, "demo", "rabbitmq password");

    DEFINE_string(quota_mqvhost, "/demo", "rabbitmq vhost");
    DEFINE_string(quota_mqExchange, "t_node", "rabbitmq exchange");
    DEFINE_string(quota_mqExchangeGroup, "t_node_grp", "rabbitmq exchange group");
    DEFINE_string(quota_mqAddress, "localhost", "rabbitmq ip address");
    DEFINE_string(quota_mqPort, "5672", "rabbitmq port");
    DEFINE_string(quota_mqUser, "demo", "rabbitmq user");
    DEFINE_string(quota_mqPassword, "demo", "rabbitmq password");
    DEFINE_string(quota_quota_mqvhost, "/demo", "rabbitmq vhost");
    DEFINE_bool(Botrading, true, "message color");
    DEFINE_string(Invalid, "Invalid", "Invalid input parameters!");
    Teconfig::Teconfig() {
    }

    Teconfig::~Teconfig() {
    }

    void Teconfig::ParseCommandLine(int* argc, char*** argv, bool cv) {
        gflags::ParseCommandLineFlags(argc, argv, cv);
    }

    bool Teconfig::Getboolconfig(const std::string& str){
        if(str == "screen")
            return FLAGS_screen;
        else if(str == "color")
            return FLAGS_color;
        else if(str == "Botrading") {
            return FLAGS_Botrading;
        } else {
            std::cout << "Invalid input parameters!" << std::endl;
            return false;
        }
    }

    const std::string& Teconfig::Getconfig(const std::string& str) {
        if(str == "loggerdir")
            return FLAGS_loggerDir;
        else if(str == "loggerfile")
            return FLAGS_loggerFile;
        else if(str == "payratio")
            return FLAGS_payRatio;
        else if(str == "id")
            return FLAGS_Id;
        else if(str == "mqvhost")
            return FLAGS_mqvhost;
        else if(str == "mqExchange")
            return FLAGS_mqExchange;
        else if(str == "mqExchangeGroup")
            return FLAGS_mqExchangeGroup;
        else if(str == "mqAddress")
            return FLAGS_mqAddress;
        else if(str == "mqPort")
            return FLAGS_mqPort;
        else if(str == "mqUser")
            return FLAGS_mqUser;
        else if(str == "mqPassword")
            return FLAGS_mqPassword;
        else if(str == "quota_mqvhost")
            return FLAGS_quota_mqvhost;
        else if(str == "quota_mqExchange")
            return FLAGS_quota_mqExchange;
        else if(str == "quota_mqExchangeGroup")
            return FLAGS_quota_mqExchangeGroup;
        else if(str == "quota_mqAddress")
            return FLAGS_quota_mqAddress;
        else if(str == "quota_mqPort")
            return FLAGS_quota_mqPort;
        else if(str == "quota_mqUser")
            return FLAGS_quota_mqUser;
        else if(str == "quota_mqPassword")
            return FLAGS_quota_mqPassword;
        else {
            std::cout << "Invalid input parameters!" << std::endl;
            return FLAGS_Invalid;
        }
    }
}
