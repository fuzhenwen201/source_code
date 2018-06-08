//
// Created by derek on 12/22/16.
//

#include <thread>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "../include/tnodeexception.h"
#include "unittest.h"


using namespace snetwork_xservice_tnode;
using namespace snetwork_xservice_tnode_unittest;

static std::atomic_int uinit_test_value;

class TestConsumer : public TNodeConsumer{
public:

    TestConsumer(){
    }

    TestConsumer(const std::string& event_name){
        event_name_ =  event_name;
        msg_count_ = 0;
    }

    ~TestConsumer(){
    }

    std::string GetEventName() {
        return  event_name_;
    }

    uint32 EventHandle(uint16 tag, char *event, uint32 eventlen) {
        std::string msg = "this is test msg";
        std::string test_msg = event;

        uinit_test_value--;

        if (50 == tag){
            if ((msg.length() + 1 ) != eventlen || test_msg != msg){
                throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
            }
        } else if (90 == tag){
            msg_count_++;
            if ((msg.length() + 1 ) != eventlen || test_msg != msg || msg_count_ > 5){
                throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
            }
        }

        return UNIVERSAL_TNODE_SUCCESS;
    }

    uint32 ServiceHandle(uint16 tag, char *reqbody, uint32 reqlen, TNodeDataSend send) {
        std::string msg = "this is test service msg";
        std::string str = "test service ack";
        char* buff;

        if (60 != tag || reqlen != (msg.length() + 1)){
            throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
        }

        std::string test_msg = reqbody;

        if (msg != test_msg){
            throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
        }

        uinit_test_value--;

        TNode tn;
        tn.Bind(2);

        TNodeChannel channel = tn.GetChannel(0);

        buff = channel.AllocDataBuff((uint32)str.length() + 1);
        memcpy(buff, str.c_str(), (uint32)(str.length() + 1));

        send(tag, buff, (uint32)(str.length() + 1));

        return UNIVERSAL_TNODE_SUCCESS;
    }

    void SetTestCount(int32 count){
        msg_count_ = count;
    }

private:
    std::string event_name_;
    int32  msg_count_;
};

class TestService : public TNodeHandle{
public:
    TestService(){

    }

    ~TestService(){

    }

    uint32 Handle(uint16 tag, char *data, uint32 len){
        std::string msg = "test service ack";

        if (60 != tag || len != (msg.length() + 1)){
            throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
        }

        std::string test_msg = data;

        if (msg != test_msg){
            throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
        }

        uinit_test_value--;

        return  0;
    }
};

class TestGroupConsumer : public TNodeConsumer{
public:

    TestGroupConsumer(){
    }

    TestGroupConsumer(std::string& event_name){
        event_name_ =  event_name;
        msg_count_ = 0;
    }

    ~TestGroupConsumer(){
    }

    std::string GetEventName(){
        return  event_name_;
    }

    uint32 EventHandle(uint16 tag, char *event, uint32 eventlen){
        std::string msg = "this is test group two msg";
        std::string test_event_2 = "TestEvent_2";
        std::string group_msg = "this is test group all msg";
        std::string group_balance_msg = "this is test group balance msg";
        std::string test_msg;
        std::string test_group_balance_event_1 = "Test_Group_Balance_Event_1";
        std::string test_group_balance_event_2 = "Test_Group_Balance_Event_2";

        uinit_test_value--;
        test_msg = event;

        if (70 == tag){
            if ((msg.length() + 1 ) != eventlen || test_msg != msg || event_name_ != test_event_2){
                throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
            }
        } else if (80 == tag){
            if ((group_msg.length() + 1 ) != eventlen || test_msg != group_msg){
                throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
            }
        } else if (100 == tag){
            msg_count_++;
            if ((group_balance_msg.length() + 1 ) != eventlen || test_msg != group_balance_msg){
                throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
            }

            if (test_group_balance_event_1 == event_name_){
                if (msg_count_ > 10){
                    throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
                }
            }else if (test_group_balance_event_2 == event_name_){
                if (msg_count_ > 5){
                    throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
                }
            }else {
                throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
            }
        } else {
            throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"test failed ");
        }

        return UNIVERSAL_TNODE_SUCCESS;
    }

    uint32 ServiceHandle(uint16 tag, char *reqbody, uint32 reqlen, TNodeDataSend send){

        return UNIVERSAL_TNODE_SUCCESS;
    }

    void SetTestCount(int32 count){
        msg_count_ = count;
    }

private:
    std::string event_name_;
    int32  msg_count_;
};


UnitTest::UnitTest(){
}

UnitTest::~UnitTest(){

}


void UnitTest::Event001(const TNodeConfigV0 &cfg){

    TNode tn;

    tn.Config(cfg);
    tn.Bind(1);
    tn.Run();

    test_count_--;
}

void UnitTest::Event002(const TNodeConfigV0 &cfg)
{
    TNode tn;
    tn.Config(cfg);
    tn.Bind(1);
    tn.Run();

    sleep(2);
    tn.Stop();

    test_count_--;
    test_A--;
}

void UnitTest::Event003(const TNodeConfigV0 &cfg){
    tnode_.Config(cfg);
    tnode_.Bind(2);
    tnode_.Run();

    test_count_--;
    test_B--;
}

void UnitTest::Event004(){
    std::string test_event = "testEvent";
    std::string test_event_2 = "testEvent2";
    TestConsumer* consumer;
    TestConsumer* consumer_2;
    int count = 3000;

    TNodeChannel channel = tnode_.GetChannel();

    do {
        consumer = new TestConsumer(test_event);
        channel.AddConsumer(consumer);

        consumer_2 = new TestConsumer(test_event_2);
        channel.AddConsumer(consumer_2);

        channel.DeleteConsumer(consumer);
        delete consumer;

        channel.DeleteConsumer(consumer_2);
        delete consumer_2;

        usleep(2000);
    }while (count--);

    uinit_test_value = 0;
}

void UnitTest::Event005(){
    std::string test_event = "TestServiceEvent";
    TestConsumer* consumer;
    TestService* service;
    int count = 3000;

    TNodeChannel channel = tnode_.GetChannel();

    do {
        consumer = new TestConsumer(test_event);
        channel.AddConsumer(consumer);

        service = new TestService();
        channel.SetServiceHandle(service);

        channel.DeleteConsumer(consumer);
        delete consumer;

        channel.ClearServiceHandle();
        delete service;

        usleep(2000);
    }while (count--);

    uinit_test_value = 0;
}

void UnitTest::Event006(){
    std::string test_event = "TestEvent_2";
    std::string test_group = "test_group";
    TestConsumer* consumer;
    int count = 3000;

    TNodeChannel channel = tnode_.GetChannel();

    do {
        consumer = new TestConsumer(test_event);

        channel.CreateGroup(test_group);
        channel.AddConsumerToGroup(test_group, consumer);

        channel.DeleteGroup(test_group);
        delete consumer;

        usleep(2000);
    }while (count--);

    uinit_test_value = 0;
}

void UnitTest::Run(){

    TestCase1();
    TestCase2();
    TestCase3();
    TestCase4();
    TestCase5();
    TestCase6();
    TestCase7();
    TestCase8();
    TestCase9();
    TestCase10();
    TestCase11();
    TestCase12();
    TestCase13();
    TestCase14();
    TestCase15();
    TestCase16();
    TestCase17();
    TestCase18();

    printf("test success end\n");
}

void UnitTest::TestCase1(){
    TNodeConfigV0 cfg;
    cfg
    .VHost("/demo")
    .Exchange("t_node")
    .ExchangeGroup("t_node_grp")
    .Address("localhost")
    .Port("5672")
    .User("demo")
    .Password("demo");

    test_count_ = 2;
    test_A = 1;

    std::thread event1([this, cfg]{Event001(cfg);});
    std::thread event2([this, cfg]{Event002(cfg);});


    event1.detach();
    event2.detach();

    TNode tn;
    tn.Config(cfg);
    tn.Bind(1);
    tn.Run();

    do {
        sleep(1);
    }while (tn.IsReady());

    if (0 != test_count_ || 0 != test_A){
        printf("count: %d, A: %d\n", (int)test_count_, test_A);
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN,"TestCase1 :: test failed ");
    }

    printf("TestCase1 :: test success\n");
}

void UnitTest::TestCase2(){
    TNodeConfigV0 cfg;
    cfg
    .VHost("/demo")
    .Exchange("t_node")
    .ExchangeGroup("t_node_grp")
    .Address("localhost")
    .Port("5672")
    .User("demo")
    .Password("demo");

    test_count_ = 2;
    test_A = 1;
    test_B = 1;

    std::thread event2([this, cfg]{Event002(cfg);});
    std::thread event3([this, cfg]{Event003(cfg);});

    event3.detach();
    event2.detach();

    TNode tn;
    tn.Config(cfg);
    tn.Bind(1);
    tn.Run();

    do {
        sleep(1);
    }while (tn.IsReady());


    if(!tnode_.IsReady()){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase2 :: test failed on step 1");
    }

    if (0 != test_count_ || 0 != test_B || 0 != test_A){
        printf("count: %d, A:%d, B:%d\n", (int)test_count_, test_A, test_B);
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase2 :: test failed on step 2");
    }

    printf("TestCase2 :: test success\n");
}

//依赖TestCase2 中 tnode_ 未 stop, 同时检查tnode_是否是有效的
void UnitTest::TestCase3(){
    std::string test_event = "testEvent";
    TestConsumer* consumer;
    std::string msg = "this is test msg";
    char * buff;
    int  time_out = 10;

    uinit_test_value = 1;

    TNodeChannel channel = tnode_.GetChannel();

    consumer = new TestConsumer(test_event);
    channel.AddConsumer(consumer);

    sleep(1);
    buff = channel.AllocDataBuff((uint32)msg.length() + 1);
    memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));

    channel.PublishEvent(test_event, buff, (uint32)(msg.length() + 1), 50);

    while (time_out--){
        if (0 == uinit_test_value){
            break;
        }
        sleep(1);
    }

    if (0 != uinit_test_value){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase3 :: test failed");
    }

    channel.DeleteConsumer(consumer);
    delete consumer;

    printf("TestCase3 :: test success\n");
}

void UnitTest::TestCase4(){
    std::string test_service = "TestServiceEvent";
    TestConsumer* consumer;
    std::string msg = "this is test service msg";
    char * buff;
    int  time_out = 10;

    uinit_test_value = 2;

    TNodeChannel channel = tnode_.GetChannel();

    consumer = new TestConsumer(test_service);

    channel.AddConsumer(consumer);

    TestService* service = new TestService();

    channel.SetServiceHandle(service);

    sleep(1);

    buff = channel.AllocDataBuff((uint32)msg.length() + 1);
    memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));

    channel.RequestService(test_service, buff, (uint32)(msg.length() + 1), 60);

    while (time_out--){
        if (0 == uinit_test_value){
            break;
        }
        sleep(1);
    }

    if (0 != uinit_test_value){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase4 :: test failed");
    }

    channel.ClearServiceHandle();
    delete service;

    channel.DeleteConsumer(consumer);
    delete consumer;

    printf("TestCase4 :: test success\n");
}


void UnitTest::TestCase5(){
    std::string test_event_1 = "TestEvent_1";
    std::string test_event_2 = "TestEvent_2";
    std::string test_event_3 = "TestEvent_3";
    std::string test_group = "test_group";
    TestGroupConsumer* consumer_one;
    TestGroupConsumer* consumer_two;
    TestGroupConsumer* consumer_three;
    std::string msg = "this is test group two msg";
    char * buff;
    int  time_out = 10;

    uinit_test_value = 1;

    TNodeChannel channel = tnode_.GetChannel();

    consumer_one = new TestGroupConsumer(test_event_1);
    consumer_two = new TestGroupConsumer(test_event_2);
    consumer_three = new TestGroupConsumer(test_event_3);

    channel.CreateGroup(test_group);

    channel.AddConsumerToGroup(test_group, consumer_one);
    channel.AddConsumerToGroup(test_group, consumer_two);
    channel.AddConsumerToGroup(test_group, consumer_three);

    sleep(1);

    buff = channel.AllocDataBuff((uint32)msg.length() + 1);
    memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));
    channel.PublishGroup(test_group, test_event_2, buff, (uint32)(msg.length() + 1), 70);

    while (time_out--){
        if (0 == uinit_test_value){
            break;
        }
        sleep(1);
    }

    if (0 != uinit_test_value){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase5 :: test failed");
    }

    channel.DeleteConsumerFromGroup(test_group, consumer_one);
    channel.DeleteConsumerFromGroup(test_group, consumer_two);
    channel.DeleteConsumerFromGroup(test_group, consumer_three);
    delete consumer_one;
    delete consumer_two;
    delete consumer_three;

    channel.DeleteGroup(test_group);

    printf("TestCase5 :: test success\n");
}


void UnitTest::TestCase6(){
    std::string test_event_1 = "Test_Event_1";
    std::string test_event_2 = "Test_Event_2";
    std::string test_event_3 = "Test_Event_3";
    std::string test_event_all = "$";
    std::string test_group = "test_group_all";
    TestGroupConsumer* consumer_one;
    TestGroupConsumer* consumer_two;
    TestGroupConsumer* consumer_three;
    std::string msg = "this is test group all msg";
    char * buff;
    int  time_out = 10;

    uinit_test_value = 3;

    TNodeChannel channel = tnode_.GetChannel();

    consumer_one = new TestGroupConsumer(test_event_1);
    consumer_two = new TestGroupConsumer(test_event_2);
    consumer_three = new TestGroupConsumer(test_event_3);

    channel.CreateGroup(test_group);

    channel.AddConsumerToGroup(test_group, consumer_one);
    channel.AddConsumerToGroup(test_group, consumer_two);
    channel.AddConsumerToGroup(test_group, consumer_three);

    sleep(1);

    buff = channel.AllocDataBuff((uint32)msg.length() + 1);
    memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));

    channel.PublishGroup(test_group, test_event_all, buff, (uint32)(msg.length() + 1), 80);

    while (time_out--){
        if (0 == uinit_test_value){
            break;
        }
        sleep(1);
    }

    if (0 != uinit_test_value){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase6 :: test failed");
    }

    channel.DeleteGroup(test_group);
    channel.DeleteConsumerFromGroup(test_group, consumer_one);
    channel.DeleteConsumerFromGroup(test_group, consumer_two);
    channel.DeleteConsumerFromGroup(test_group, consumer_three);
    delete consumer_one;
    delete consumer_two;
    delete consumer_three;



    printf("TestCase6 :: test success\n");
}

void UnitTest::TestCase7(){
    std::string test_event = "testEvent";
    TestConsumer* consumer;
    TestConsumer* consumer_2;
    std::string msg = "this is test msg";
    char * buff;
    int  time_out = 10;
    int  msg_cnt = 10;

    uinit_test_value = 10;

    TNode tn;
    TNodeConfigV0 cfg;
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


    TNodeChannel channel = tn.GetChannel(3);

    consumer = new TestConsumer(test_event);
    channel.AddConsumer(consumer);

    TNodeChannel channel_2 = tnode_.GetChannel(3);
    consumer_2 = new TestConsumer(test_event);
    channel_2.AddConsumer(consumer_2);

    sleep(1);

    while (msg_cnt--){
        buff = channel.AllocDataBuff((uint32)msg.length() + 1);
        memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));

        channel.PublishEvent(test_event, buff, (uint32)(msg.length() + 1), 90);
    }

    while (time_out--){
        if (0 == uinit_test_value){
            break;
        }
        sleep(1);
    }

    if (0 != uinit_test_value){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase7 :: test failed");
    }

    channel.DeleteConsumer(consumer);
    channel_2.DeleteConsumer(consumer_2);
    delete  consumer;
    delete  consumer_2;

    printf("TestCase7 :: test success\n");
}

void UnitTest::TestCase8(){
    std::string test_event_1 = "Test_Group_Balance_Event_1";
    std::string test_event_2 = "Test_Group_Balance_Event_2";
    std::string test_event_all = "$";
    std::string test_group = "test_group_Balance_all";
    TestGroupConsumer* consumer_one;
    TestGroupConsumer* consumer_two;
    TestGroupConsumer* consumer_three;
    std::string msg = "this is test group balance msg";
    char * buff;
    int  msg_cnt = 10;
    int  time_out = 10;

    TNode tn1;
    TNode tn2;
    TNode tn3;
    TNode tn4;
    TNodeConfigV0 cfg;

    cfg
    .VHost("/demo")
    .Exchange("t_node")
    .ExchangeGroup("t_node_grp")
    .Address("192.168.64.128")
    .Port("5672")
    .User("demo")
    .Password("demo");

    uinit_test_value = 20;

    tn1.Config(cfg);
    tn1.Bind(1);
    tn1.Run();

    tn2.Config(cfg);
    tn2.Bind(2);
    tn2.Run();

    tn3.Config(cfg);
    tn3.Bind(3);
    tn3.Run();

    tn4.Config(cfg);
    tn4.Bind(4);
    tn4.Run();


    TNodeChannel channel1 = tn1.GetChannel(1);
    channel1.CreateGroup(test_group);
    consumer_one = new TestGroupConsumer(test_event_1);
    consumer_one->SetTestCount(0);
    channel1.AddConsumerToGroup(test_group, consumer_one);

    TNodeChannel channel2 = tn2.GetChannel(2);
    channel2.CreateGroup(test_group);
    consumer_two = new TestGroupConsumer(test_event_2);
    consumer_two->SetTestCount(0);
    channel2.AddConsumerToGroup(test_group, consumer_two);

    TNodeChannel channel3 = tn3.GetChannel(3);
    channel3.CreateGroup(test_group);
    consumer_three = new TestGroupConsumer(test_event_2);
    consumer_three->SetTestCount(0);
    channel3.AddConsumerToGroup(test_group, consumer_three);

    TNodeChannel channel4 = tn4.GetChannel(4);

    sleep(1);
    while (msg_cnt--){
        buff = channel4.AllocDataBuff((uint32)msg.length() + 1);
        memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));

        channel4.PublishGroup(test_group, test_event_all, buff, (uint32)(msg.length() + 1), 100);
    }

    while (time_out--){
        if (0 == uinit_test_value){
            break;
        }
        sleep(1);
    }

    if (0 != uinit_test_value){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase8 :: test failed");
    }

    channel1.DeleteGroup(test_group);
    delete consumer_one;

    channel2.DeleteGroup(test_group);
    delete consumer_two;

    channel3.DeleteGroup(test_group);
    delete consumer_three;

    printf("TestCase8 :: test success\n");
}

//依赖测试用例8
void UnitTest::TestCase9(){
    TNode tn4;

    if (tn4.Check()){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase9 :: test failed");
    }

    tn4.Bind(4);

    if (!tn4.Check() || !tn4.IsReady()){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase9 :: test failed");
    }

    tn4.UnBind(4);

    if (tn4.Check()){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase9 :: test failed");
    }

    printf("TestCase9 :: test success\n");
}


void UnitTest::TestCase10(){
    TNodeChannel channel = tnode_.GetChannel(0);
    char *buff;

    buff = channel.AllocDataBuff(255);

    channel.FreeDataBuff(buff);

    printf("TestCase10 :: test success\n");
}

void UnitTest::TestCase11(){
    try {
        TNodeChannel channel = tnode_.GetChannel(5);
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase11 :: test failed");
    }catch (TNodeException e){
        if (e.GetErrorCode() == UNIVERSAL_TNODE_PARAMETER_ERR){
            printf("TestCase11 :: test success\n");
        } else {
            throw e;
        }
    }
}


void UnitTest::TestCase12(){
    TNodeChannel channel = tnode_.GetChannel();
    std::string test_event = "testEvent";
    char buff[30];

    try {
        channel.PublishEvent(test_event, buff, 30, 26);
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase12 :: test failed");
    }catch (TNodeException e){
        if (e.GetErrorCode() == UNIVERSAL_TNODE_INVALID_MSG_BUFF_ERR){
            printf("TestCase12 :: test success\n");
        } else {
            throw e;
        }
    }
}

void UnitTest::TestCase13(){
    TNodeChannel channel = tnode_.GetChannel();
    std::string test_event = "testEvent";


    char buff[30];

    try {
        channel.RequestService(test_event, buff, 30, 26);
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase13 :: test failed");
    }catch (TNodeException e){
        if (e.GetErrorCode() == UNIVERSAL_TNODE_INVALID_MSG_BUFF_ERR){
            printf("TestCase13 :: test success\n");
        } else {
            throw e;
        }
    }
}

void UnitTest::TestCase14(){
    TNodeChannel channel = tnode_.GetChannel();
    std::string test_event = "testEvent";
    std::string test_group = "test_group_Balance_all";
    char buff[30];

    try {
        channel.PublishGroup(test_group, test_event, buff, 30, 26);
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase14 :: test failed");
    }catch (TNodeException e){
        if (e.GetErrorCode() == UNIVERSAL_TNODE_INVALID_MSG_BUFF_ERR){
            printf("TestCase14 :: test success\n");
        } else {
            throw e;
        }
    }
}

void UnitTest::TestCase15(){
    std::string test_event = "testEvent";
    std::string msg = "this is test msg";
    char * buff;

    uinit_test_value = 1;

    TNodeChannel channel = tnode_.GetChannel();
    std::thread event4([this]{Event004();});
    event4.detach();

    sleep(1);

    while (uinit_test_value){
        buff = channel.AllocDataBuff((uint32)msg.length() + 1);
        memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));

        channel.PublishEvent(test_event, buff, (uint32)(msg.length() + 1), 50);
        usleep(2000);
    }

    if (0 != uinit_test_value){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase15 :: test failed");
    }

    printf("TestCase15 :: test success\n");
}


void UnitTest::TestCase16(){
    std::string test_event = "TestServiceEvent";
    std::string msg = "this is test service msg";
    char * buff;

    uinit_test_value = 1;

    TNodeChannel channel = tnode_.GetChannel();
    std::thread event5([this]{Event005();});
    event5.detach();

    sleep(1);

    while (uinit_test_value){
        buff = channel.AllocDataBuff((uint32)msg.length() + 1);
        memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));

        channel.RequestService(test_event, buff, (uint32)(msg.length() + 1), 60);
        usleep(2000);
    }

    if (0 != uinit_test_value){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase16 :: test failed");
    }

    printf("TestCase16 :: test success\n");
}

void UnitTest::TestCase17(){
    std::string test_event = "TestEvent_2";
    std::string msg = "this is test group two msg";
    std::string test_group = "test_group";
    char * buff;

    uinit_test_value = 1;

    TNodeChannel channel = tnode_.GetChannel();
    std::thread event6([this]{Event006();});
    event6.detach();

    sleep(1);

    while (uinit_test_value){
        buff = channel.AllocDataBuff((uint32)msg.length() + 1);
        memcpy(buff, msg.c_str(), (uint32)(msg.length() + 1));

        channel.PublishGroup(test_group, test_event, buff, (uint32)(msg.length() + 1), 70);
        usleep(2000);
    }

    if (0 != uinit_test_value){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase17 :: test failed");
    }

    printf("TestCase17 :: test success\n");
}

void UnitTest::TestCase18(){
    TNode tn;

    tn.Bind(1);
    tn.UnBind(1);

    if(tn.Check()){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase18 :: test failed");
    }

    tn.Bind(2);
    tn.UnBind(2);
    if(tn.Check()){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase18 :: test failed");
    }

    tn.Bind(3);
    tn.UnBind(3);
    if(tn.Check()){
        throw UNIVERSAL_TNODE_EXCEPTION(UNIVERSAL_TNODE_UNKNOWN, "TestCase18 :: test failed");
    }

    printf("TestCase18 :: test success\n");
}
