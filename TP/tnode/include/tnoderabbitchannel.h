//
// Created by alex on 12/22/16.
//

#ifndef TNODE_RABBIT_CHANNEL_H
#define TNODE_RABBIT_CHANNEL_H

#include "tnodechannel.h"

#include <list>
#include <atomic>
#include <mutex>
#include <amqpcpp.h>
#include "snetwork_typedef.h"
#include "tnodehandle.h"
#include "tnodeconsumer.h"
#include "tnodelog.h"

namespace snetwork_xservice_tnode
{
    typedef std::list<TNodeConsumer*>  TNodeListConsumer;

    enum TNodeChannelMsgType {
        TNODE_EVENT_MSG_TYPE = 0,
        TNODE_SERVICE_MSG_TYPE,
        TNODE_GROUP_MSG_TYPE
    };

    struct TNodeCnEventHeader {
        uint8  flag;
        uint8  msg_type;
        uint16 tag;
        uint32 msg_len;
        uint32 pad;
    };

    struct TNodeCnServiceHeader {
        uint8  flag;
        uint8  msg_type;
        uint16 tag;
        uint32 queue_name_len;
        uint32 msg_len;
    };

    struct TNodeGroup{
        std::string group_name;
        TNodeListConsumer* consumer_list;
    };

    typedef std::list<TNodeGroup*>  TNodeListGroup;



    class TNodeRabbitChannel {
    public:
        TNodeRabbitChannel(AMQP::Channel *channel, std::string &exchange, std::string &group_exchange);

        ~TNodeRabbitChannel();

        void SetServiceHandle(TNodeHandle *response_handle);

        void ClearServiceHandle();

        char*  AllocDataBuff(uint32 data_len);
        void   FreeDataBuff(const char* data);

        void PublishEvent(const std::string &eventname, const char *data, uint32 datalen, uint16 tag);

        void RequestService(const std::string &eventname, const char *data, uint32 datalen, uint16 tag);

        void PublishGroup(const std::string &groupname, const std::string &eventname, const char *data, uint32 datalen, uint16 tag);

        void CreateGroup(const std::string &groupname);
        void DeleteGroup(const std::string &groupname);

        void AddConsumerToGroup(const std::string &groupname, TNodeConsumer *consumer);
        void DeleteConsumerFromGroup(const std::string &groupname, TNodeConsumer *consumer);

        void AddConsumer(TNodeConsumer *consumer);

        void DeleteConsumer(TNodeConsumer *consumer);

    private:
        AMQP::Channel *channel_;
        std::atomic<TNodeHandle*> response_handle_;
        std::string exchange_;
        std::string group_exchange_;

        std::string queuename_;
        std::mutex mutex_;

        TNodeListGroup group_list_;
        TNodeListConsumer consumer_list_;

        uint8 check_flag_;
    };
}


#endif //TNODE_RABBIT_CHANNEL_H
