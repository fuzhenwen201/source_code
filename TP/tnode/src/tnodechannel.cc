//
// Created by alex on 12/29/16.
//

//
// Created by alex on 12/15/16.
//

#include <assert.h>
#include "tnodechannel.h"
#include "tnoderabbitchannel.h"

using namespace snetwork_xservice_tnode;


TNodeChannel::TNodeChannel(TNodeRabbitChannel* rabbitChannel) {
    assert(nullptr != rabbitChannel);

    rabbitChannel_ = rabbitChannel;
}

TNodeChannel::~TNodeChannel() {

}

TNodeChannel::TNodeChannel(const TNodeChannel &channel) {
    rabbitChannel_ = channel.rabbitChannel_;
}

TNodeChannel &TNodeChannel::operator=(const TNodeChannel &channel ){
    if (this == &channel)
    {
        return *this;
    }

    this->rabbitChannel_ = channel.rabbitChannel_;

    return *this;
}

void TNodeChannel::PublishEvent(const std::string &eventname, const char *data, uint32 datalen, uint16 tag) {

    rabbitChannel_->PublishEvent(eventname, data, datalen, tag);
}

void TNodeChannel::SetServiceHandle(TNodeHandle *response_handle) {
    rabbitChannel_->SetServiceHandle(response_handle);
}

void TNodeChannel::ClearServiceHandle() {
    rabbitChannel_->ClearServiceHandle();
}

char*  TNodeChannel::AllocDataBuff(uint32 data_len){
    return rabbitChannel_->AllocDataBuff(data_len);
}

void   TNodeChannel::FreeDataBuff(const char* data){
    rabbitChannel_->FreeDataBuff(data);
}

void TNodeChannel::RequestService(const std::string &eventname, const char *data, uint32 datalen, uint16 tag) {
    rabbitChannel_->RequestService(eventname, data, datalen, tag);
}

void TNodeChannel::PublishGroup(const std::string &groupname, const std::string &eventname, const char *data, uint32 datalen,
                                        uint16 tag) {
    rabbitChannel_->PublishGroup(groupname, eventname, data, datalen, tag);
}

void TNodeChannel::CreateGroup(const std::string &groupname) {

    rabbitChannel_->CreateGroup(groupname);
}

void TNodeChannel::DeleteGroup(const std::string &groupname) {

    rabbitChannel_->DeleteGroup(groupname);
}


void TNodeChannel::AddConsumerToGroup(const std::string &groupname, TNodeConsumer *consumer){
    rabbitChannel_->AddConsumerToGroup(groupname, consumer);
}


void TNodeChannel::DeleteConsumerFromGroup(const std::string &groupname, TNodeConsumer *consumer){
    rabbitChannel_->DeleteConsumerFromGroup(groupname, consumer);
}

void TNodeChannel::AddConsumer(TNodeConsumer *consumer) {

    rabbitChannel_->AddConsumer(consumer);
}

void TNodeChannel::DeleteConsumer(TNodeConsumer *consumer) {

    rabbitChannel_->DeleteConsumer(consumer);
}

