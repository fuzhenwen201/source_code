//
// Created by alex on 12/15/16.
//

#ifndef TNODE_HANDLE_H
#define TNODE_HANDLE_H

#include "snetwork_typedef.h"

namespace snetwork_xservice_tnode
{

    class TNodeHandle {
    public:
        TNodeHandle(){

        }

        virtual ~TNodeHandle(){

        }

        virtual uint32 Handle(uint16 tag, char *data, uint32 len) = 0;
    };

}

#endif //TNODE_HANDLE_H
