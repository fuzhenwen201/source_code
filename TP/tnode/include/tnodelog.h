//
// Created by alex on 12/29/16.
//

#ifndef TNODE_LOG_H
#define TNODE_LOG_H

#include <string>
#include "snetwork_typedef.h"

namespace snetwork_xservice_tnode
{

    class TNodeLog {
    public:
        TNodeLog();
        ~TNodeLog();

        static void Log(const char* message);
        static void OpLog(const char* message);
        static void ErrLog(const char* message);
    };

}

#endif //TNODE_LOG_H
