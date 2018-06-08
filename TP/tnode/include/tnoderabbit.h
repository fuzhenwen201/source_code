//
// Created by alex on 12/22/16.
//

#ifndef TNODE_RABBIT_H
#define TNODE_RABBIT_H

#include <string>
#include <mutex>
#include <thread>
#include <ev.h>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <unistd.h>
#include "tnodeerrno.h"
#include "snetwork_typedef.h"
#include "tnodeconfig.h"
#include "tnodechannel.h"
#include "tnoderabbitchannel.h"
#include "tnodelog.h"
#include "tnodeexception.h"

namespace snetwork_xservice_tnode
{
    typedef struct RabbitConfigStruct{
        std::string vhost;
        std::string exchange;
        std::string exchange_grp;
        std::string address;
        std::string user;
        std::string password;
        std::string port;
    }RabbitConfig;

    typedef struct ChannelManagerStruct{
        TNodeRabbitChannel *channel_ptr;
        AMQP::Channel *amqp_channel_ptr;
        uint32  error;
    }ChannelManager;

    class TNodeTcpConnection : public AMQP::TcpConnection{
    public:
        TNodeTcpConnection(AMQP::TcpHandler *handler, const AMQP::Address &address) : AMQP::TcpConnection(handler, address){

        }

        ~TNodeTcpConnection(){

        };

        uint16_t onNegotiate(AMQP::Connection *connection, uint16_t interval){
            //return 0 for closed  heartbeat
            return 0;
        }
    };

    class TNodeRabbit {
    public:
        TNodeRabbit();

        ~TNodeRabbit();

        void Config(RabbitConfig& config);

        void Run();
        void Stop();
        bool IsReady();

        TNodeChannel GetChannel();
        TNodeChannel GetChannel(uint32 channel_id);

    private:
        std::string vhost_;
        std::string exchange_;
        std::string exchange_grp_;
        std::string address_;
        std::string user_;
        std::string password_;
        std::string port_;
        uint16 config_channel_max_num_;
        uint32 config_sleep_us_;
        uint32 pos_;

        std::mutex mutex_;
        ChannelManager* channel_manager_ptr_;

        AMQP::LibEvHandler  *phandler_;
        TNodeTcpConnection *pconnection_;

        std::thread* thread_ptr_;
        bool    thread_run_flag_;
        bool    thread_end_flag_;

        int32  timeout_;
        std::atomic_int init_count_;

        bool    ready_;
        bool    is_run_;
    };

}

#endif //TNODE_RABBIT_H
