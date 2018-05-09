/*************************************************************************
    > File Name: selfconfig.h
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Fri 15 Dec 2017 02:38:56 AM PST
 ************************************************************************/

#ifndef __SELF_CONFIG_H__
#define __SELF_CONFIG_H__

#include "gflags/gflags.h"
#include "glog/logging.h"

using std::string;
namespace snetwork_xservice_config {
    class SelfConfig {
    public:
        SelfConfig();
        ~SelfConfig();
        void ParseCommandLine(int* argc, char*** argv, bool cv);
        void InitGoogleLogging(char** argv);
        const std::string& GetStringconfig(const std::string& str);
        bool Getboolconfig(const std::string& str);
        int32_t Getintconfig(const std::string& str);
    };
}
#endif //__SELF_CONFIG_H__
