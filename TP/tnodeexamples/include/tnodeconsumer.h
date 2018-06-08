//
// Created by alex on 12/15/16.
//

#ifndef TNODE_CONSUMER_H
#define TNODE_CONSUMER_H

#include <string>
#include <functional>
#include "snetwork_typedef.h"


namespace snetwork_xservice_tnode
{
    using TNodeDataSend  =   std::function<void(uint16_t tag, char *data, uint32_t size)>;

    class TNodeConsumer {
    public:
        virtual std::string GetEventName() = 0;

        virtual uint32 EventHandle(uint16 tag, char *event, uint32 eventlen) = 0;

        virtual uint32 ServiceHandle(uint16 tag, char *reqbody, uint32 reqlen, TNodeDataSend send) = 0;
    };

}

#endif //TNODE_CONSUMER_H
