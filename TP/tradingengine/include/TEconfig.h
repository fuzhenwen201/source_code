
#ifndef __TE_CONDIF_H__
#define __TE_CONFIG_H__
#include <gflags/gflags.h>

namespace snetwork_xservice_xflagger {

class Teconfig {
    public:
        Teconfig();
        virtual ~Teconfig();
        void ParseCommandLine(int* argc, char*** argv, bool cv);
        const std::string& Getconfig(const std::string& str);
        bool Getboolconfig(const std::string& str);
};

}
#endif
