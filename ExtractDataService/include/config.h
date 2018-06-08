
#ifndef __CONDIF_H__
#define __CONFIG_H__
#include <gflags/gflags.h>

namespace snetwork_xservice_xflagger {

class Config {
    public:
        Config();
        virtual ~Config();
        void ParseCommandLine(int* argc, char*** argv, bool cv);
        const std::string& Getconfig(const std::string& str);
        bool Getboolconfig(const std::string& str);
        int Getintconfig(const std::string& str);
};

}
#endif
