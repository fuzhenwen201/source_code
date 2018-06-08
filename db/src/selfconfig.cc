/*************************************************************************
    > File Name: selfconfig.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Fri 15 Dec 2017 02:36:50 AM PST
 ************************************************************************/

#include "selfconfig.h"

namespace snetwork_xservice_selfconfig {
    DEFINE_bool(logtostderr, true, "printf message on terminator");
    DEFINE_bool(colorlogtostderr, true, "message color");
    DEFINE_string(log_dir, "./logs", "logger directory");
    DEFINE_int32(logbufsecs, 0, "0 refers to real-time outpu");
    DEFINE_int32(maxlogsize,2048, "Set the maximum log file size");
    DEFINE_string(ordertopic, "order_topic", "rocketmq topic");
    DEFINE_string(producernamesrv, "10.10.10.22:9876", "rocketmq server ip and port");
    DEFINE_string(producergroup, "producer_group", "rocketmq group");
    DEFINE_string(consumernamesrv, "10.10.10.31:9876", "rocketmq server ip and port");
    DEFINE_string(consumergroup, "consumer_group", "rocketmq group");
    DEFINE_string(snapshottopic, "snapshot_topic", "rocketmq topic");
    DEFINE_string(ineffectual,"ineffectual","ineffectual");

    SelfConfig::SelfConfig() {}
    SelfConfig::~SelfConfig() {}

    void SelfConfig::ParseCommandLine(int* argc, char*** argv, bool cv) {
        gflags::ParseCommandLineFlags(argc, argv, cv);
    }

    void SelfConfig::InitGoogleLogging(char** argv){
        google::InitGoogleLogging(argv[0]);
    }

    bool SelfConfig::Getboolconfig(const std::string& str){
        if(str == "logtostderr")
            return FLAGS_logtostderr;
        else if(str == "colorlogtostderr")
            return FLAGS_colorlogtostderr;
        else
            return false;
    }

    int32_t SelfConfig::Getintconfig(const std::string& str){
        if(str == "logbufsecs")
            return FLAGS_logbufsecs;
        else if(str == "maxlogsize")
            return FLAGS_maxlogsize;
        else
            return 0;
    }

    const std::string& SelfConfig::GetStringconfig(const std::string& str){
        if(str == "log_dir")
            return FLAGS_log_dir;
        else if (str == "ordertopic")
            return FLAGS_ordertopic;
        else if(str == "consumernamesrv")
            return FLAGS_consumernamesrv;
        else if(str == "consumergroup")
            return FLAGS_consumergroup;
        else if(str == "producernamesrv")
            return FLAGS_producernamesrv;
        else if(str == "producergroup")
            return FLAGS_producergroup;
        else if(str == "snapshottopic")
            return FLAGS_snapshottopic;
        else
            return FLAGS_ineffectual;
    }
}
