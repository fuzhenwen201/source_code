#include <iostream>
#include "config.h"

namespace snetwork_xservice_xflagger {
    DEFINE_bool(screen, false, "printf message on terminator");
    DEFINE_bool(color, true, "message color");
    DEFINE_string(loggerDir, "./log", "logger directory");
    DEFINE_string(loggerFile, "tradingengine", "logger file name");

    DEFINE_string(mqvhost, "/demo", "rabbitmq vhost");
    DEFINE_string(mqExchange, "t_node", "rabbitmq exchange");
    DEFINE_string(mqExchangeGroup, "t_node_grp", "rabbitmq exchange group");
    DEFINE_string(mqAddress, "localhost", "rabbitmq ip address");
    DEFINE_int32(mqPort, 5672, "rabbitmq port");
    DEFINE_string(mqUser, "demo", "rabbitmq user");
    DEFINE_string(mqPassword, "demo", "rabbitmq password");
    DEFINE_string(mysqluser, "root", "Connect to the database user name.");
    DEFINE_string(mysqlpwd, "root", "Connect to the database password.");
    DEFINE_string(mysqlurl, "tcp://127.0.0.1:3306", "Connect to the database url");
    DEFINE_int32(maxsize, 2, "The size of the database connection pool.");

    DEFINE_string(redisIP, "localhost", "redis ip address");
    DEFINE_int32(redisPort, 6379, "redis port");
    DEFINE_string(redisPass, "blockchain", "redis password");
    DEFINE_string(Invalid, "Invalid", "Invalid input parameters!");
    Config::Config() {
    }

    Config::~Config() {
    }

    void Config::ParseCommandLine(int* argc, char*** argv, bool cv) {
        gflags::ParseCommandLineFlags(argc, argv, cv);
    }

    bool Config::Getboolconfig(const std::string& str){
        if(str == "screen")
            return FLAGS_screen;
        else if(str == "color")
            return FLAGS_color;
        else {
            std::cout << "Invalid input parameters!" << std::endl;
            return false;
        }
    }

    int Config::Getintconfig(const std::string& str){
        if(str == "mqPort")
            return FLAGS_mqPort;
        else if(str == "maxsize")
            return FLAGS_maxsize;
        else if(str == "redisPort")
            return FLAGS_redisPort;
        else {
            std::cout << "Invalid input parameters!" << std::endl;
            return 0;
        }
    }

    const std::string& Config::Getconfig(const std::string& str) {
        if(str == "loggerdir")
            return FLAGS_loggerDir;
        else if(str == "loggerfile")
            return FLAGS_loggerFile;
        else if(str == "mqvhost")
            return FLAGS_mqvhost;
        else if(str == "mqExchange")
            return FLAGS_mqExchange;
        else if(str == "mqExchangeGroup")
            return FLAGS_mqExchangeGroup;
        else if(str == "mqAddress")
            return FLAGS_mqAddress;
        else if(str == "mqUser")
            return FLAGS_mqUser;
        else if(str == "mqPassword")
            return FLAGS_mqPassword;
        else if(str == "mysqluser")
            return FLAGS_mysqluser;
        else if(str == "mysqlpwd")
            return FLAGS_mysqlpwd;
        else if(str == "mysqlurl")
            return FLAGS_mysqlurl;
        else if(str == "redisIP")
            return FLAGS_redisIP;
        else if(str == "redisPass")
            return FLAGS_redisPass;
        else {
            std::cout << "Invalid input parameters!" << std::endl;
            return FLAGS_Invalid;
        }
    }
}
