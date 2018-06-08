//
// Created by alex on 12/15/16.
//

#include <assert.h>
#include "tools.h"
#include "tnodeerrno.h"
#include "tnoderabbitchannel.h"
#include "tnodeexception.h"

//add by savin
#include "ReadDataProcessor.h"


using namespace snetwork_xservice_tnode;


TNodeRabbitChannel::TNodeRabbitChannel(AMQP::Channel *channel, std::string &exchange, std::string &group_exchange) {
    assert(nullptr != channel);

    channel_ = channel;
    response_handle_ = nullptr;
    exchange_ = exchange;
    group_exchange_ = group_exchange;
    queuename_ = uuid();

    check_flag_ = 0x8f;
}

TNodeRabbitChannel::~TNodeRabbitChannel() {
    TNodeGroup* group_ptr = nullptr;
    TNodeListConsumer* consumer_list_ptr;
    TNodeConsumer* consumer;
    std::string queuename;
    std::string group_key;


    mutex_.lock();

    if(nullptr != response_handle_){
        channel_->removeQueue(queuename_);
        channel_->unbindQueue(exchange_, queuename_, queuename_);
        channel_->cancel(queuename_);
    }

    response_handle_ = nullptr;

    for (TNodeListGroup::iterator it = group_list_.begin(); it != group_list_.end();) {

        group_ptr = *it;
        group_list_.erase(it++);

        consumer_list_ptr =  group_ptr->consumer_list;
        group_key = group_ptr->group_name + ".$";

        for (TNodeListConsumer::iterator item = consumer_list_ptr->begin(); item != consumer_list_ptr->end();){
            consumer = *item;

            queuename = group_ptr->group_name + "." + consumer->GetEventName();

            channel_->removeQueue(queuename);
            channel_->unbindQueue(group_exchange_, queuename, group_key);
            channel_->unbindQueue(group_exchange_, queuename, queuename);
            channel_->cancel(queuename);

            consumer_list_ptr->erase(item++);
        }

        delete group_ptr->consumer_list;
        delete group_ptr;
    }

    for (TNodeListConsumer::iterator it = consumer_list_.begin(); it != consumer_list_.end();) {
        consumer = (*it);

        queuename = consumer->GetEventName();

        channel_->removeQueue(queuename);
        channel_->unbindQueue(exchange_, queuename, queuename);
        channel_->cancel(queuename);

        consumer_list_.erase(it++);
    }

    mutex_.unlock();

    channel_ = nullptr;
}

void TNodeRabbitChannel::PublishEvent(const std::string &eventname, const char *data, uint32 datalen, uint16 tag) {
    uint32 send_len;
    TNodeCnEventHeader* header_ptr;

    assert(channel_ != nullptr);

    if (nullptr == data || 0 == datalen) {
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_NULL_PTR_ERR, "data is nullptr or datalen is zero");
    }

    header_ptr = (TNodeCnEventHeader*)(data - sizeof(TNodeCnEventHeader));
    if (check_flag_ != header_ptr->flag){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_INVALID_MSG_BUFF_ERR, "must be alloc data buffer with AllocDataBuff");
    }

    header_ptr->msg_type = TNODE_EVENT_MSG_TYPE;
    header_ptr->tag = htobe16(tag);
    header_ptr->msg_len = htobe32(datalen);
    header_ptr->pad = 0;

    send_len = datalen + sizeof(TNodeCnEventHeader);

    channel_->publish(exchange_, eventname, (const char *) header_ptr, send_len);

    delete header_ptr;
}

void TNodeRabbitChannel::SetServiceHandle(TNodeHandle *response_handle) {
    assert(nullptr != response_handle);

    mutex_.lock();

    if (nullptr == response_handle_) {
        response_handle_ =  response_handle;

        channel_->declareQueue(queuename_, AMQP::autodelete).onSuccess([this](const std::string &name,
          int msgcount,
          int consumercount){

            channel_->bindQueue(exchange_, queuename_, queuename_);
            channel_->consume(queuename_, AMQP::noack).onReceived([this](const AMQP::Message &message,
                                                                     uint64_t deliveryTag,
                                                                     bool redelivered) {
                const char *pmsg;
                uint32 msg_size;
                TNodeCnEventHeader *pheader;

                pmsg = message.body();
                msg_size = (uint32) message.bodySize();

                if (msg_size < sizeof(TNodeCnEventHeader)) {
                    TNodeLog::ErrLog("invalid service msg");
                    return;
                }

                pheader = (TNodeCnEventHeader *) pmsg;
                if(check_flag_ != pheader->flag){
                    TNodeLog::ErrLog("error service msg");
                    return;
                }

                pheader->tag = be16toh(pheader->tag);
                pheader->msg_len = be32toh(pheader->msg_len);

                if (TNODE_EVENT_MSG_TYPE != pheader->msg_type ||
                    msg_size != (sizeof(TNodeCnEventHeader) + pheader->msg_len)) {
                    TNodeLog::ErrLog("error service msg header");
                    return;
                }

                ((TNodeHandle *)response_handle_)->Handle(pheader->tag, (char *)(pmsg + sizeof(TNodeCnEventHeader)), pheader->msg_len);

            }).onError([](const char *message){
                TNodeLog::ErrLog(message);
            });
        }).onError([](const char *message){
            TNodeLog::ErrLog(message);
        });
    } else {
        response_handle_ = response_handle;
    }

    mutex_.unlock();
}

void TNodeRabbitChannel::ClearServiceHandle() {
    mutex_.lock();

    if(nullptr != response_handle_){
        channel_->removeQueue(queuename_);
        channel_->unbindQueue(exchange_, queuename_, queuename_);
        channel_->cancel(queuename_);
    }

    response_handle_ = nullptr;

    mutex_.unlock();
}

void TNodeRabbitChannel::RequestService(const std::string &eventname, const char *data, uint32 datalen, uint16 tag) {
    uint32 send_len;
    uint32 queue_name_Len;
    TNodeCnServiceHeader* header_ptr;

    assert(channel_ != nullptr);

    if (nullptr == data || 0 == datalen) {
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_NULL_PTR_ERR, "data is nullptr or datalen is zero");
    }

    header_ptr = (TNodeCnServiceHeader*)(data - sizeof(TNodeCnServiceHeader));
    if (check_flag_ != header_ptr->flag){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_INVALID_MSG_BUFF_ERR, "must be alloc data buffer with AllocDataBuff");
    }

    header_ptr->msg_type = TNODE_SERVICE_MSG_TYPE;
    header_ptr->tag = htobe16(tag);

    queue_name_Len = (uint32) queuename_.length() + 1;
    header_ptr->queue_name_len = htobe32(queue_name_Len);
    header_ptr->msg_len = htobe32(datalen);

    send_len = datalen + sizeof(TNodeCnServiceHeader) + queue_name_Len;

    memcpy((char*)data + datalen, queuename_.c_str(), queue_name_Len);

    channel_->publish(exchange_, eventname, (const char *) header_ptr, send_len);


    delete header_ptr;

}

void TNodeRabbitChannel::PublishGroup(const std::string &groupname, const std::string &eventname, const char *data, uint32 datalen,
                                   uint16 tag) {
    uint32 len;
    TNodeCnEventHeader* header_ptr;

    assert(channel_ != nullptr);

    if (nullptr == data || 0 == datalen) {
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_NULL_PTR_ERR, "data is nullptr or datalen is zero");
    }

    header_ptr = (TNodeCnEventHeader*)(data - sizeof(TNodeCnEventHeader));
    if(check_flag_ != header_ptr->flag){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_INVALID_MSG_BUFF_ERR, "must be alloc data buffer with AllocDataBuff");
    }

    header_ptr->msg_type = TNODE_GROUP_MSG_TYPE;
    header_ptr->tag = htobe16(tag);
    header_ptr->msg_len = htobe32(datalen);

    len = datalen + sizeof(TNodeCnEventHeader);

    //channel_->publish(group_exchange_, groupname + "." + eventname, (const char *) header_ptr, len);

	char pSrc[4096]= {0};  
	memcpy(pSrc, eventname.c_str(),4096-1);
	char *pSRC = pSrc ;

	if (NULL == strstr(pSrc,".") )
	{
		channel_->publish(group_exchange_, groupname + "." + eventname, (const char *) header_ptr, len);
		
	}
	else
	{
		char *pNew = NULL;
		char *pFind = NULL;
		while((pFind = strtok_r(pSRC, ".", &pNew)) != NULL)
		{
			channel_->publish(group_exchange_, groupname + "." + pFind, (const char *) header_ptr, len);
			pSRC = NULL;
		}
	}

    delete header_ptr;
}

void TNodeRabbitChannel::CreateGroup(const std::string &groupname){
    TNodeGroup* group_ptr;

    mutex_.lock();

    for (TNodeListGroup::iterator it = group_list_.begin(); it != group_list_.end(); it++) {
        if (groupname == (*it)->group_name) {
            mutex_.unlock();
            throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_GROUP_REPEAT_ERR, "conflict group");
        }
    }

    group_ptr = new TNodeGroup();
    group_ptr->group_name = groupname;
    group_ptr->consumer_list = new TNodeListConsumer;

    group_list_.push_back(group_ptr);

    mutex_.unlock();
}

void TNodeRabbitChannel::DeleteGroup(const std::string &groupname){
    TNodeGroup* group_ptr = nullptr;
    TNodeListConsumer* consumer_list_ptr;
    TNodeConsumer* consumer;
    std::string queuename;
    std::string group_key;

    mutex_.lock();

    for (TNodeListGroup::iterator it = group_list_.begin(); it != group_list_.end(); it++) {
        if (groupname == (*it)->group_name) {
            group_ptr = *it;
            group_list_.erase(it);
            break;
        }
    }

    if (nullptr != group_ptr){
        consumer_list_ptr =  group_ptr->consumer_list;
        group_key = groupname + ".$";

        for (TNodeListConsumer::iterator it = consumer_list_ptr->begin(); it != consumer_list_ptr->end();){
            consumer = *it;

            queuename = groupname + "." + consumer->GetEventName();

            channel_->removeQueue(queuename);
            channel_->unbindQueue(group_exchange_, queuename, group_key);
            channel_->unbindQueue(group_exchange_, queuename, queuename);
            channel_->cancel(queuename);

            consumer_list_ptr->erase(it++);
        }

        delete group_ptr->consumer_list;
        delete group_ptr;
    }

    mutex_.unlock();
}


void TNodeRabbitChannel::AddConsumerToGroup(const std::string &groupname, TNodeConsumer *consumer){
    TNodeGroup* group_ptr = nullptr;
    TNodeListConsumer* consumer_list_ptr;
    std::string queuename;
    std::string group_key;

	

    if (nullptr == consumer) {
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_NULL_PTR_ERR, "AddConsumerToGroup : consumer is nullptr");
    }

    mutex_.lock();

    for (TNodeListGroup::iterator it = group_list_.begin(); it != group_list_.end(); it++) {
        if (groupname == (*it)->group_name) {
            group_ptr = *it;
            break;
        }
    }

    if (nullptr != group_ptr){
        consumer_list_ptr =  group_ptr->consumer_list;
        group_key = groupname + ".$";
        queuename = groupname + "." + consumer->GetEventName();
        consumer_list_ptr->push_back(consumer);

        channel_->declareQueue(queuename, AMQP::autodelete).onSuccess([this, queuename, group_key, consumer](const std::string &name,
                                                                                                              int msgcount,
                                                                                                              int consumercount) {
            channel_->bindQueue(group_exchange_, queuename, queuename);
            channel_->bindQueue(group_exchange_, queuename, group_key);

            channel_->consume(queuename, AMQP::noack).onReceived([this, consumer](const AMQP::Message &message,
                                                                            uint64_t deliveryTag,
                                                                            bool redelivered) {

                const char *pmsg;
                uint32 msg_size;
                TNodeCnEventHeader *pheader;

                pmsg = message.body();
                msg_size = (uint32) message.bodySize();

                if (msg_size < sizeof(TNodeCnEventHeader)) {
                    TNodeLog::ErrLog("invalid group event msg");
                    return;
                }

                pheader = (TNodeCnEventHeader *) pmsg;
                if(check_flag_ != pheader->flag){
                    TNodeLog::ErrLog("error group event msg");
                    return;
                }

                pheader->tag = be16toh(pheader->tag);
                pheader->msg_len = be32toh(pheader->msg_len);

                if (TNODE_GROUP_MSG_TYPE != pheader->msg_type
                    || msg_size != (sizeof(TNodeCnEventHeader) + pheader->msg_len)) {
                    TNodeLog::ErrLog("error group event msg header");
                    return;
                }

				// modify by savin
				//CReadDataProcessor::PushReadData(pheader->tag, (char *) (pmsg + sizeof(TNodeCnEventHeader)), pheader->msg_len,consumer);
                consumer->EventHandle(pheader->tag, (char *) (pmsg + sizeof(TNodeCnEventHeader)), pheader->msg_len);

                
            }).onError([](const char *message){
                TNodeLog::ErrLog(message);
            });
        }).onError([](const char *message){
            TNodeLog::ErrLog(message);
        });
    }

    mutex_.unlock();
}

void TNodeRabbitChannel::DeleteConsumerFromGroup(const std::string &groupname, TNodeConsumer *consumer){
    TNodeGroup* group_ptr = nullptr;
    TNodeListConsumer* consumer_list_ptr;
    std::string queuename;
    std::string group_key;

    mutex_.lock();

    for (TNodeListGroup::iterator it = group_list_.begin(); it != group_list_.end(); it++) {
        if (groupname == (*it)->group_name) {
            group_ptr = *it;
            break;
        }
    }

    if (nullptr != group_ptr){
        consumer_list_ptr =  group_ptr->consumer_list;
        for (TNodeListConsumer::iterator it = consumer_list_ptr->begin(); it != consumer_list_ptr->end(); it++) {
            if (consumer == (*it)) {
                group_key = groupname + ".$";
                queuename = groupname + "." + (*it)->GetEventName();

                channel_->removeQueue(queuename);
                channel_->unbindQueue(group_exchange_, queuename, group_key);
                channel_->unbindQueue(group_exchange_, queuename, queuename);
                channel_->cancel(queuename);

                consumer_list_ptr->erase(it);
                break;
            }
        }
    }

    mutex_.unlock();
}

void TNodeRabbitChannel::AddConsumer(TNodeConsumer *consumer) {
    std::string event_name;

    if (nullptr == consumer) {
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_NULL_PTR_ERR, "consumer is nullptr");
    }

    event_name = consumer->GetEventName();

    mutex_.lock();

    for (TNodeListConsumer::iterator it = consumer_list_.begin(); it != consumer_list_.end(); it++) {
        if (event_name == (*it)->GetEventName()) {
            mutex_.unlock();
            throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_CONSUMER_REPEAT_ERR, "conflict consumer");
        }
    }

    channel_->declareQueue(event_name, AMQP::autodelete).onSuccess([this, consumer, event_name](const std::string &name,
                                                     int msgcount,
                                                     int consumercount) {
        channel_->bindQueue(exchange_, event_name, event_name);
        channel_->consume(event_name, AMQP::noack).onReceived(
                [this, consumer](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {
                    const char *pmsg;
                    uint32 msg_size;
                    TNodeCnEventHeader *pheader;
                    TNodeCnServiceHeader *psrv_header;
                    std::string queue_name;

                    pmsg = message.body();
                    msg_size = (uint32) message.bodySize();

                    if (msg_size < sizeof(TNodeCnEventHeader)) {
                        TNodeLog::ErrLog("invalid event msg");
                        return;
                    }

                    pheader = (TNodeCnEventHeader *) pmsg;
                    if(check_flag_ != pheader->flag){
                        TNodeLog::ErrLog("error event msg");
                        return;
                    }

                    if (TNODE_EVENT_MSG_TYPE == pheader->msg_type) {
                        pheader->tag = be16toh(pheader->tag);
                        pheader->msg_len = be32toh(pheader->msg_len);

                        if (msg_size != (sizeof(TNodeCnEventHeader) + pheader->msg_len)) {
                            TNodeLog::ErrLog("error event msg header");
                            return;
                        }

						
						// modify by savin
						//CReadDataProcessor::PushReadData(pheader->tag, (char *) (pmsg + sizeof(TNodeCnEventHeader)),pheader->msg_len,consumer);
						
						
                        consumer->EventHandle(pheader->tag, (char *) (pmsg + sizeof(TNodeCnEventHeader)), pheader->msg_len);
						

                    } else if (TNODE_SERVICE_MSG_TYPE == pheader->msg_type) {
                        psrv_header = (TNodeCnServiceHeader *) pmsg;
                        psrv_header->tag = be16toh(psrv_header->tag);
                        psrv_header->queue_name_len = be32toh(psrv_header->queue_name_len);
                        psrv_header->msg_len = be32toh(psrv_header->msg_len);

                        if (msg_size !=
                            (sizeof(TNodeCnServiceHeader) + psrv_header->queue_name_len + psrv_header->msg_len)) {
                            TNodeLog::ErrLog("error service msg header");
                            return;
                        }

                        queue_name = pmsg + sizeof(TNodeCnServiceHeader) + psrv_header->msg_len;

                        consumer->ServiceHandle(psrv_header->tag, (char *)(pmsg + sizeof(TNodeCnServiceHeader)), psrv_header->msg_len,
                                [this, &queue_name](uint16 tag, char * data, uint32 len){
                                    if (nullptr == data || 0 == len){
                                       return;
                                    }

                                    PublishEvent(queue_name, data, len, tag);

                                    return;
                                });
                        } else {
                            TNodeLog::ErrLog("unknown msg header");
                            return;
                        }
                }
        );
    });

    consumer_list_.push_back(consumer);

    mutex_.unlock();
}

void TNodeRabbitChannel::DeleteConsumer(TNodeConsumer *consumer) {
    std::string queuename;

    if (nullptr == consumer) {
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_NULL_PTR_ERR, "consumer is nullptr");
    }

    mutex_.lock();

    for (TNodeListConsumer::iterator it = consumer_list_.begin(); it != consumer_list_.end(); it++) {
        if (consumer == (*it)) {
            queuename = (*it)->GetEventName();

            channel_->removeQueue(queuename);
            channel_->unbindQueue(exchange_, queuename, queuename);
            channel_->cancel(queuename);

            consumer_list_.erase(it);
            break;
        }
    }

    mutex_.unlock();
}


char*  TNodeRabbitChannel::AllocDataBuff(uint32 data_len){
    char* data;

    if (0 == data_len){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TNode unknown state");
    }

    data = new char[data_len + queuename_.length() + sizeof(TNodeCnEventHeader) + 1];          //1 是字符串结束符  1 is c_str ‘\0’

    *data = check_flag_;

    return  data + sizeof(TNodeCnEventHeader);
}

void  TNodeRabbitChannel::FreeDataBuff(const char* data){

    if (nullptr == data){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_NULL_PTR_ERR, "data is nullptr");
    }

    data = (data - sizeof(TNodeCnEventHeader));
    if ((uint8)(*data) != check_flag_){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_INVALID_MSG_BUFF_ERR, "invalid data buff");
    }

    delete  data;
}
