//
// Created by alex on 12/16/16.
//

#include "tnoderabbit.h"

using namespace snetwork_xservice_tnode;


TNodeRabbit::TNodeRabbit() {
    pconnection_ = nullptr;
    phandler_ = nullptr;

    config_channel_max_num_ = 5;
    config_sleep_us_ = 500000;
    init_count_ = 2 * config_channel_max_num_;
    pos_ = 0;
    timeout_ = 6;

    ready_ = false;
    is_run_ = false;

    thread_ptr_ = nullptr;
    thread_run_flag_ = false;
    thread_end_flag_ = true;

    channel_manager_ptr_ = new ChannelManager[config_channel_max_num_];
    memset(channel_manager_ptr_, 0, sizeof(ChannelManager) * config_channel_max_num_);
}

TNodeRabbit::~TNodeRabbit() {

    Stop();

    if (nullptr != pconnection_){
        delete  pconnection_;
        pconnection_ = nullptr;
    }

    if (nullptr != phandler_){
        delete phandler_;
        phandler_ = nullptr;
    }

    if (nullptr != thread_ptr_){
        delete  thread_ptr_;
        thread_ptr_ = nullptr;
    }

    if (nullptr != channel_manager_ptr_){
        delete  channel_manager_ptr_;
        channel_manager_ptr_ = nullptr;
    }
}

void TNodeRabbit::Config(RabbitConfig& config) {
    mutex_.lock();

    vhost_ = config.vhost;
    exchange_ = config.exchange;
    exchange_grp_ = config.exchange_grp;
    address_ = config.address;
    port_ = config.port;
    user_ = config.user;
    password_ = config.password;

    mutex_.unlock();
}

void TNodeRabbit::Run() {
    int32 timeout = timeout_;

    mutex_.lock();
    if (is_run_){
        mutex_.unlock();
        return;
    }

    is_run_ = true;
    init_count_ = 2 * config_channel_max_num_;

    thread_ptr_ = new std::thread([this] {
        AMQP::Channel* amqp_ch;
        uint32 id;
        std::string url;

        auto *loop = ev_loop_new (EVBACKEND_EPOLL | EVFLAG_NOENV);

        url = "amqp://" + user_ + ":" + password_ + "@" + address_ + ":" + port_ + "/" + vhost_;


        mutex_.lock();
        phandler_ = new AMQP::LibEvHandler(loop);
        pconnection_ = new TNodeTcpConnection(phandler_, AMQP::Address(url));

        for (id = 0; id < config_channel_max_num_; id++){
            amqp_ch = new AMQP::TcpChannel(pconnection_);
            channel_manager_ptr_[id].channel_ptr = new TNodeRabbitChannel(amqp_ch, exchange_, exchange_grp_);
            channel_manager_ptr_[id].amqp_channel_ptr = amqp_ch;

            amqp_ch->onReady([this, id, amqp_ch](){
                TNodeLog::Log("channel connect");

                channel_manager_ptr_[id].error = UNIVERSAL_TNODE_SUCCESS;

                amqp_ch->declareExchange(exchange_, AMQP::direct).onSuccess([this]() {
                    init_count_--;
                }).onError([this, id](const char *message){

                    channel_manager_ptr_[id].error = UNIVERSAL_TNODE_CHANNEL_EXCHANGE_DEFINE_ERR;

                    TNodeLog::ErrLog(message);
                    throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_CHANNEL_EXCHANGE_DEFINE_ERR,
                                                         message);
                });

                amqp_ch->declareExchange(exchange_grp_, AMQP::topic).onSuccess([this]() {
                    init_count_--;
                }).onError([this, id](const char *message){
                    channel_manager_ptr_[id].error = UNIVERSAL_TNODE_CHANNEL_EXCHANGE_GRP_DEFINE_ERR;

                    TNodeLog::ErrLog(message);
                    throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_CHANNEL_EXCHANGE_GRP_DEFINE_ERR,
                                                         message);
                });

                amqp_ch->onError([](const char *message){
                    TNodeLog::ErrLog(message);
                });
            });
        }

        thread_run_flag_ = true;
        thread_end_flag_ = false;
        mutex_.unlock();

        ev_run(loop, 0);
        ev_loop_destroy(loop);

        thread_end_flag_ = true;
    });

    thread_ptr_->detach();
    mutex_.unlock();

    while (init_count_ > 0){
        usleep(config_sleep_us_);
        timeout--;

        if (timeout <= 0){
            throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_RUN_TIMEOUT_ERR, "TNode run failed");
        }
    }

    ready_ = true;
}

void TNodeRabbit::Stop(){
    uint32 id;

    do {
        mutex_.lock();

        if (!is_run_){
            mutex_.unlock();
            return;
        }

        if (!thread_run_flag_){
            mutex_.unlock();

            usleep(config_sleep_us_);
        }else {
            break;
        }
    }while (1);

    pconnection_->close();

    while (!thread_end_flag_){
        usleep(config_sleep_us_);
    }

    for(id = 0; id < config_channel_max_num_; id++){
        delete channel_manager_ptr_[id].channel_ptr;
        delete channel_manager_ptr_[id].amqp_channel_ptr;

        channel_manager_ptr_[id].channel_ptr = nullptr;
        channel_manager_ptr_[id].amqp_channel_ptr = nullptr;
        channel_manager_ptr_[id].error = 0;
    }

    delete pconnection_;
    pconnection_ = nullptr;

    delete phandler_;
    phandler_ = nullptr;

    delete thread_ptr_;
    thread_ptr_ = nullptr;

    thread_run_flag_ = false;
    ready_ =    false;
    is_run_ =   false;

    mutex_.unlock();
}

TNodeChannel TNodeRabbit::GetChannel() {
    TNodeRabbitChannel* rabbit_channel_ptr = nullptr;
    uint32 i;

    mutex_.lock();

    if (ready_){
        for (i = 0; i < config_channel_max_num_; i++){
            if (UNIVERSAL_TNODE_SUCCESS == channel_manager_ptr_[pos_].error){
                rabbit_channel_ptr = channel_manager_ptr_[pos_].channel_ptr;
                pos_ = (pos_ + 1) % config_channel_max_num_;
                break;
            }

            pos_ = (pos_ + 1) % config_channel_max_num_;
        }

        mutex_.unlock();
    } else {
        mutex_.unlock();

        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_NOT_READY_ERR, "TNode is not ready");
    }

    if (nullptr != rabbit_channel_ptr){
        TNodeChannel channel(rabbit_channel_ptr);
        return channel;
    }else {
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TNode unknown state");

        return  TNodeChannel(nullptr);
    }
}

TNodeChannel TNodeRabbit::GetChannel(uint32 channel_id) {
    TNodeRabbitChannel* rabbit_channel_ptr = nullptr;

    if (channel_id >= config_channel_max_num_){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_PARAMETER_ERR, "error id");
    }

    mutex_.lock();

    if (ready_){
        rabbit_channel_ptr = channel_manager_ptr_[channel_id].channel_ptr;

        mutex_.unlock();
    } else {
        mutex_.unlock();

        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_NOT_READY_ERR, "TNode is not ready");
    }

    if (nullptr != rabbit_channel_ptr){
        TNodeChannel channel(rabbit_channel_ptr);
        return channel;
    }else {
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TNode unknown state");

        return TNodeChannel(nullptr);
    }
}

bool TNodeRabbit::IsReady(){
    return ready_;
}


