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
        TNodeChannel(TNodeRabbitChannel* rabbitChannel);

        ~TNodeChannel();

        void SetServiceHandle(TNodeHandle *response_handle);

        void ClearServiceHandle();

        char*  AllocDataBuff(uint32 data_len);
        void   FreeDataBuff(char* data);

        void PublishEvent(std::string &eventname, char *data, uint32 datalen, uint16 tag);

        void RequestService(std::string &eventname, char *data, uint32 datalen, uint16 tag);

        void PublishGroup(std::string &groupname, std::string &eventname, char *data, uint32 datalen, uint16 tag);

        void CreateGroup(std::string &groupname);
        void DeleteGroup(std::string &groupname);

        void AddConsumerToGroup(std::string &groupname, TNodeConsumer *consumer);
        void DeleteConsumerFromGroup(std::string &groupname, TNodeConsumer *consumer);

        void AddConsumer(TNodeConsumer *consumer);

        void DeleteConsumer(TNodeConsumer *consumer);

    private:
        TNodeRabbitChannel* rabbitChannel_;
    };
}

#endif //TNODE_CHANNEL_H
