#include <iostream>
#include <string.h>
#include <zconf.h>
#include "tnode.h"
#include "tnodeconfig_v0.h"

using namespace snetwork_xservice_tnode;
using namespace std;

class TestConsumer : public TNodeConsumer{
public:

    TestConsumer(){
    }

    TestConsumer(std::string& event_name){
        event_name_ =  event_name;
    }

    ~TestConsumer(){
    }

    std::string GetEventName(){
        return  event_name_;
    }

    uint32 EventHandle(uint16 tag, char *event, uint32 eventlen){
        std::string test_msg = event;

        cout << "receive message tag is " << tag << endl;

        cout << "receive message length is " << eventlen << endl;

        cout << "Receive message : " << test_msg << endl;

        return UNIVERSAL_TNODE_SUCCESS;
    }

    uint32 ServiceHandle(uint16 tag, char *reqbody, uint32 reqlen, TNodeDataSend send){
        std::string msg = "Hello, Alex!";
        std::string ack_msg = "Hello, Lukesh!";
        std::string test_msg = reqbody;
        char* buff;

        if (60 != tag) {
            cout << "receive message tag is " << tag << ", but expected " << 60 << endl;
            return UNIVERSAL_TNODE_UNKNOWN;
        }

        if ((msg.length() + 1 ) != reqlen){
            cout << "receive message length is " << reqlen << ", but expected " << (msg.length() + 1 ) << endl;
            return UNIVERSAL_TNODE_UNKNOWN;
        }

        if (test_msg != msg){
            cout << "receive message is : " << test_msg << ", but expected " << msg << endl;
            return UNIVERSAL_TNODE_UNKNOWN;
        }

        cout << "receive message, alex will send ack message : " << ack_msg << endl;

        TNode tn;
        tn.Bind(1);

        TNodeChannel channel = tn.GetChannel(0);

        buff = channel.AllocDataBuff((uint32)ack_msg.length() + 1);
        memcpy(buff, ack_msg.c_str(), (uint32)(ack_msg.length() + 1));
        send(tag, buff, (uint32)(ack_msg.length() + 1));

        return UNIVERSAL_TNODE_SUCCESS;
    }

private:
    std::string event_name_;
};

class TestGroupConsumer : public TNodeConsumer{
public:

    TestGroupConsumer(){
    }

    TestGroupConsumer(std::string& event_name){
        event_name_ =  event_name;
    }

    ~TestGroupConsumer(){
    }

    std::string GetEventName(){
        return  event_name_;
    }

    uint32 EventHandle(uint16 tag, char *event, uint32 eventlen){
        std::string test_msg = event;

        cout << "receive message tag is " << tag << endl;

        cout << "receive message length is " << eventlen << endl;

        cout << "Receive message : " << test_msg << endl;

        return UNIVERSAL_TNODE_SUCCESS;
    }

    uint32 ServiceHandle(uint16 tag, char *reqbody, uint32 reqlen, TNodeDataSend send){

        return UNIVERSAL_TNODE_SUCCESS;
    }

private:
    std::string event_name_;
};

class TestService : public TNodeHandle{
public:
    TestService(){

    }

    ~TestService(){

    }

    uint32 Handle(uint16 tag, char *data, uint32 len){
        std::string test_msg = data;

        cout << "receive ACK message tag is " << tag << endl;

        cout << "receive ACK message length is " << len << endl;

        cout << "Receive ACK message : " << test_msg << endl;

        return  0;
    }
};


int main(int argc, char *argv[]) {
    TNode tn;
    std::string cmd;
    std::string subcribe = "consumer";
    std::string subcribeGroup = "group_consumer";
    std::string publish = "publish";
    std::string publish_Service = "publish_Service";
    std::string publish_Group = "publish_Group";
    std::string help = "help";
    std::string group;
    std::string event;
    std::string msg;
    uint16 tag;
    TNodeConfigV0 cfg;
    char *buff;


    if (argc < 2){
        cout << "expect parameters more than two" << endl;
        return -1;
    }

    cmd = argv[1];

    if (help == cmd){
        cout << "command list: " << endl;
        cout << "tnodeexamples consumer event" << endl;
        cout << "tnodeexamples group_consumer event group" << endl;
        cout << "tnodeexamples publish event msg tag" << endl;
        cout << "tnodeexamples publish_Service event" << endl;
        cout << "tnodeexamples publish_Group event group msg tag" << endl;
        exit(0);
    }

    cfg
    .VHost("/demo")
    .Exchange("t_node")
    .ExchangeGroup("t_node_grp")
    .Address("localhost")
    .Port("5672")
    .User("demo")
    .Password("demo");

    tn.Config(cfg);
    tn.Bind(1);
    tn.Run();

    TNodeChannel channel = tn.GetChannel();

    if (subcribe == cmd){
        if (argc < 3){
            cout << "expect parameters more than three" << endl;
            return -1;
        }

        event = argv[2];
        TestConsumer *consumer = new TestConsumer(event);
        channel.AddConsumer(consumer);
    }else if (subcribeGroup == cmd){
        if (argc < 4){
            cout << "expect parameters more than four" << endl;
            return -1;
        }

        event = argv[2];
        group = argv[3];

        channel.CreateGroup(group);
        TestConsumer *consumer = new TestConsumer(event);
        channel.AddConsumerToGroup(group, consumer);
    }else if (publish == cmd){
        if (argc < 5){
            cout << "expect parameters more than five" << endl;
            return -1;
        }

        event = argv[2];
        msg = argv[3];
        tag = (uint16)atoi(argv[4]);

        while (1){
            buff = channel.AllocDataBuff((uint32)msg.length() + 1);
            memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));
            channel.PublishEvent(event, buff, (uint32)(msg.length() + 1), tag);
            sleep(1);
        }
    }else if (publish_Service == cmd){
        if (argc < 3){
            cout << "expect parameters more than three" << endl;
            return -1;
        }

        event = argv[2];
        msg = "Hello, Alex!";
        tag = 60;

        TestService* service = new TestService();
        channel.SetServiceHandle(service);

        while (1){
            buff = channel.AllocDataBuff((uint32)msg.length() + 1);
            memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));
            channel.RequestService(event, buff, (uint32)(msg.length() + 1), tag);
            sleep(1);
        }
    }else if (publish_Group == cmd){
        if (argc < 6){
            cout << "expect parameters more than six" << endl;
            return -1;
        }

        event = argv[2];
        group = argv[3];
        msg = argv[4];
        tag = (uint16)atoi(argv[5]);

        while (1){
            buff = channel.AllocDataBuff((uint32)msg.length() + 1);
            memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));
            channel.PublishGroup(group, event, buff, (uint32)(msg.length() + 1), tag);
            sleep(1);
        }
    }

    while (1){
        sleep(5);
    }

    return 0;
}