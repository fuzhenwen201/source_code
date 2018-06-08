//
// Created by alex on 12/22/16.
//
#include "tnode.h"
#include "tnoderabbitmanager.h"

// add by savin
#include "ReadDataProcessor.h"


using namespace snetwork_xservice_tnode;

TNode::TNode() {
    id_ = 0;
    config_ = nullptr;
    rabbit_ = nullptr;
}

TNode::~TNode() {
    if (nullptr != config_){
        delete config_;
    }
}

TNode::TNode(const TNode &tnode) {
    if (nullptr != tnode.config_){
        config_ = new RabbitConfig;
        config_->address = tnode.config_->address;
        config_->exchange = tnode.config_->exchange;
        config_->exchange_grp = tnode.config_->exchange_grp;

        config_->vhost =  tnode.config_->vhost;
        config_->user = tnode.config_->user;
        config_->password =  tnode.config_->password;
        config_->port = tnode.config_->port;
    } else {
        config_ = tnode.config_;
    }

    id_ = tnode.id_;
    rabbit_ = tnode.rabbit_;
}

// add by savin
extern int g_nReadDataProcessor ;
void TNode::SetReadThreadNums(int nReadThreads)
{
	g_nReadDataProcessor = nReadThreads;
}

extern int g_nRecvBufSize;
void TNode::SetRecvBufSize(int nRecvBufSize)
{
    g_nRecvBufSize = nRecvBufSize;
}



TNode &TNode::operator=(const TNode &tnode ){
    if (this == &tnode )
    {
        return *this;
    }

    if (nullptr != tnode.config_){
        if (nullptr == config_){
            config_ = new RabbitConfig;
        }

        config_->address = tnode.config_->address;
        config_->exchange = tnode.config_->exchange;
        config_->exchange_grp = tnode.config_->exchange_grp;

        config_->vhost =  tnode.config_->vhost;
        config_->user = tnode.config_->user;
        config_->password =  tnode.config_->password;
        config_->port = tnode.config_->port;
    } else {
        if (nullptr != config_){
            delete config_;
        }

        config_ = tnode.config_;
    }

    id_ = tnode.id_;
    rabbit_ = tnode.rabbit_;

    return *this;
}

void TNode::Config(const TNodeConfig &cfg){
    if (nullptr == config_){
        config_ = new RabbitConfig;
    }

    config_->vhost =  cfg.GetConfigItem("vhost");
    config_->exchange =  cfg.GetConfigItem("exchange");
    config_->exchange_grp =  cfg.GetConfigItem("exchange_grp");

    config_->address =  cfg.GetConfigItem("address");
    config_->user =  cfg.GetConfigItem("user");
    config_->password =  cfg.GetConfigItem("password");
    config_->port = cfg.GetConfigItem("port");
}

void TNode::Bind(uint32 id){
    if (nullptr != rabbit_){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_BOUND_ERR, "TNode bind method has been executed");
    }

    rabbit_ = TNodeRabbitManager::GetRabbit(id);
    id_ = id;

    if (nullptr != config_){
        rabbit_->Config(*config_);
    }
}

void TNode::UnBind(uint32 id){
    if (nullptr == rabbit_){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNBIND_ERR, "TNode bind method not executed");
    }

    TNodeRabbitManager::FreeRabbit(id);
    id_ = 0;
    rabbit_ = nullptr;
}


void TNode::Run(){
    if (nullptr == rabbit_){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNBIND_ERR, "TNode bind method not executed");
    }

	//add by savin
	//CReadDataProcessor::StartBatchSender();

    rabbit_->Run();
}

void TNode::Stop(){
    if (nullptr == rabbit_){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNBIND_ERR, "TNode bind method not executed");
    }

    rabbit_->Stop();
}

bool TNode::Check(){
    return TNodeRabbitManager::Check(id_, rabbit_);
}

TNodeChannel TNode::GetChannel(){
    if (nullptr == rabbit_){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNBIND_ERR, "TNode bind method not executed");
    }

    return  rabbit_->GetChannel();
}

TNodeChannel TNode::GetChannel(uint32 channel_id){
    if (nullptr == rabbit_){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNBIND_ERR, "TNode bind method not executed");
    }

    return  rabbit_->GetChannel(channel_id);
}

bool TNode::IsReady(){
    if (nullptr == rabbit_){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNBIND_ERR, "TNode bind method not executed");
    }

    return rabbit_->IsReady();
}



