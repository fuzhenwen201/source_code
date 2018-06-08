//
// Created by alex on 12/29/16.
//

#ifndef TNODE_CHANNEL_H
#define TNODE_CHANNEL_H

#include <string>
#include "snetwork_typedef.h"
#include "tnodehandle.h"
#include "tnodeconsumer.h"

namespace snetwork_xservice_tnode
{
    class TNodeRabbitChannel;

    class TNodeChannel {
    public:
        explicit TNodeChannel(TNodeRabbitChannel* rabbitChannel);

        TNodeChannel(const TNodeChannel &channel);

        TNodeChannel &operator=(const TNodeChannel &channel);

        ~TNodeChannel();

        void SetServiceHandle(TNodeHandle *response_handle);

        void ClearServiceHandle();

        char*  AllocDataBuff(uint32 data_len);
        void   FreeDataBuff(const char* data);

        void PublishEvent(const std::string &eventname, const char *data, uint32 datalen, uint16 tag);

        void RequestService(const std::string &eventname, const char *data, uint32 datalen, uint16 tag);

        bool PublishGroup(const std::string &groupname, const std::string &eventname, const char *data, uint32 datalen, uint16 tag);

        void CreateGroup(const std::string &groupname);
        void DeleteGroup(const std::string &groupname);

        void AddConsumerToGroup(const std::string &groupname, TNodeConsumer *consumer);
        void DeleteConsumerFromGroup(const std::string &groupname, TNodeConsumer *consumer);

        void AddConsumer(TNodeConsumer *consumer);

        void DeleteConsumer(TNodeConsumer *consumer);

    private:
        TNodeRabbitChannel* rabbitChannel_;
    };
}

#endif //TNODE_CHANNEL_H
