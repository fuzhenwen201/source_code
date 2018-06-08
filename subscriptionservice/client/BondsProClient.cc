/*************************************************************************
    > File Name: BondsProClient.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Thu 11 Jan 2018 12:19:19 PM CST
 ************************************************************************/
#include "BondsProClient.h"
#include "quickfix/fix44/SuspensionService.h"
#include "quickfix/fix44/OrderStatusRequest.h"
#include "quickfix/fix44/MarketDataIncrementalRefresh.h"
#include <fstream>
#include "guuid.h"

static bool islogon = false;

static FIX::SessionID m_sessionID = {};
struct CancelOrderInfo {
    std::string OrgOrderID;
    std::string ExcelID;
    std::string OrderID;
    std::string Symbol;
    std::string Side;
    std::string price;
};
struct OrderStatusReq {
    std::string Accountid;
    std::string OrderID;
    std::string meOrderID;
    std::string symbol;
};

struct OrderRetranReq{
    std::string Accountid;
    std::string OrderID;
    std::string symbol;
};

static CancelOrderInfo cancelOrderInfo = {};
static OrderStatusReq orderStatusReq = {};
static OrderRetranReq orderRetranReq = {};
static OrderStatusReq hiStoryReq = {};

Applications::Applications(const FIX::SessionSettings& set):settings_(set) {
}

void Applications::onCreate( const FIX::SessionID& sessionID){
    m_sessionID = sessionID;
    std::cout << "oncreate:" << sessionID << std::endl;
}

void Applications::toAdmin(FIX::Message& message, const FIX::SessionID& sessionID) {
    const FIX::Dictionary& dictionary = settings_.get(sessionID);
    const std::string& field = FIELD_GET_REF( message.getHeader(), MsgType);
    if(FIX::MsgType_Logon == field) {
        FIX::Username username = dictionary.getString("Username");
        FIX::Password password = dictionary.getString("Password");

        message.setField(FIX::Username(username));
        message.setField(FIX::Password(password));
        message.setField(FIX::HeartBtInt(240));
        message.setField(FIX::EncryptMethod(0));
        message.setField(FIX::ResetSeqNumFlag(true));
    }
    std::cout << "1 ADMIN OUT: " << message << std::endl;
}

void Applications::fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionID)
    throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {
    std::cout << "2 ADMIN IN: " << message << std::endl;
}

void Applications::onLogon(const FIX::SessionID& sessionID) {
    islogon = true;
    std::cout << "3 Logon - " << sessionID << std::endl;
}

void Applications::fromApp(const FIX::Message& message, const FIX::SessionID& sessionID)
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType ) {
    std::cout << "4 IN:  " << message << std::endl;
    crack( message, sessionID );
}

void Applications::toApp(FIX::Message& message, const FIX::SessionID& sessionID)
    throw( FIX::DoNotSend ) {
    try {
        FIX::PossDupFlag possDupFlag;
        message.getHeader().getField( possDupFlag );
        if ( possDupFlag ) throw FIX::DoNotSend();
    } catch (FIX::FieldNotFound& ) {}

    std::cout << "5 OUT: " << message << std::endl;
}

void Applications::onLogout(const FIX::SessionID& sessionID) {
    std::cout << "6 Logout - " << sessionID << std::endl;
}

void Applications::onMessage ( const FIX44::MarketDataSnapshotFullRefresh & message, const FIX::SessionID& ) {
   std::cout << message.getField(55) << " " << message.getField(60) << "\n";
}

void Applications::onMessage ( const FIX44::ExecutionReport & message, const FIX::SessionID& ) {

}
void Applications::onMessage ( const FIX44::HistQuotResp & message, const FIX::SessionID& ){
    std::cout << "HistQuotResp:"  << FIELD_GET_REF( message.getHeader(), MsgType) << "\n";
}

void Applications::onMessage ( const FIX44::OrdRetransResp & message, const FIX::SessionID& ){
    std::cout << "OrdRetransResp:" << FIELD_GET_REF( message.getHeader(), MsgType) << "\n";
}

void Applications::onMessage ( const FIX44::SuspenService & message, const FIX::SessionID& ) {
    std::cout << "SuspenService:" << FIELD_GET_REF( message.getHeader(), MsgType) << "\n";
}

static void SendBuyOrderInfo(){
    auto u_id_ptr = new utils::unique_id_t();
    u_id_ptr->set_epoch(uint64_t(0));
    u_id_ptr->set_machine(int32_t(0));
    long param = u_id_ptr->generate();

    auto orderID = std::to_string(param);

    srand((unsigned)time(nullptr));
    int a = rand()%1000000;
    double price = (double)a/1000000 ;

    int32_t b = rand()%1000000;
    double number = (double)b/1000000;

    FIX44::NewOrderSingle newOrderSingle;
    newOrderSingle.setField(FIX::Account("1"));
    newOrderSingle.setField(FIX::ClOrdID(orderID));
    newOrderSingle.setField(FIX::Symbol("BTC/RKT"));
    newOrderSingle.setField(FIX::OrdType('2'));
    newOrderSingle.setField(FIX::Side('1'));
    newOrderSingle.setField(FIX::Price(price));
    newOrderSingle.setField(FIX::OrderQty(number));
    newOrderSingle.setField(FIX::TransactTime(true));
    FIX::Session::sendToTarget(newOrderSingle, m_sessionID);
}

static void SendSellOrderInfo(){
    auto u_id_ptr = new utils::unique_id_t();
    u_id_ptr->set_epoch(uint64_t(0));
    u_id_ptr->set_machine(int32_t(0));
    long param = u_id_ptr->generate();

    auto orderID = std::to_string(param);

    srand((unsigned)time(nullptr));
    int a = rand()%1000000;
    double price = (double)a/1000000 ;

    int32_t b = rand()%1000000;
    double number = (double)b/1000000;

    FIX44::NewOrderSingle newOrderSingle;
    newOrderSingle.setField(FIX::Account("1"));
    newOrderSingle.setField(FIX::ClOrdID(orderID));
    newOrderSingle.setField(FIX::Symbol("BTC/RKT"));
    newOrderSingle.setField(FIX::OrdType('2'));
    newOrderSingle.setField(FIX::Side('2'));
    newOrderSingle.setField(FIX::Price(price));
    newOrderSingle.setField(FIX::OrderQty(number));
    newOrderSingle.setField(FIX::TransactTime(true));
    FIX::Session::sendToTarget(newOrderSingle, m_sessionID);
}

static void SendCancelOrder(){
    FIX44::OrderCancelRequest ordercancel;
    ordercancel.setField(11, "0");
    ordercancel.setField(41, cancelOrderInfo.OrgOrderID);
    ordercancel.setField(17, cancelOrderInfo.ExcelID);
    ordercancel.setField(37, cancelOrderInfo.OrderID);
    ordercancel.setField(55, cancelOrderInfo.Symbol);
    ordercancel.setField(54, cancelOrderInfo.Side);
    ordercancel.setField(44, cancelOrderInfo.price);

    FIX::Session::sendToTarget(ordercancel, m_sessionID);
}

static void SendOrderStatReq(){
    FIX44::OrderStatusRequest orderStatusRequest;
    orderStatusRequest.setField(1,orderStatusReq.Accountid);
    orderStatusRequest.setField(11,orderStatusReq.OrderID);
    orderStatusRequest.setField(37,orderStatusReq.meOrderID);
    orderStatusRequest.setField(55,orderStatusReq.symbol);
    FIX::Session::sendToTarget(orderStatusRequest, m_sessionID);
}

static void SendrestranReq(){
    FIX44::OrdRetransReq ordRetransReq;

    ordRetransReq.setField(1,orderRetranReq.Accountid);
    ordRetransReq.setField(37, orderRetranReq.OrderID);
    ordRetransReq.setField(55, orderRetranReq.symbol);
    FIX::Session::sendToTarget(ordRetransReq, m_sessionID);
}

static void SendHistoryReq() {
    FIX44::HistQuotReq histQuotReq;
    histQuotReq.setField(1,hiStoryReq.Accountid);
    histQuotReq.setField(11,hiStoryReq.OrderID);
    histQuotReq.setField(37,hiStoryReq.meOrderID);
    histQuotReq.setField(55,hiStoryReq.symbol);
    FIX::Session::sendToTarget(histQuotReq, m_sessionID);
}

void Applications::run(){

    while (true){

        if(!islogon) {
            sleep(5);
            continue;
        }

        std::string str1;
        std::cout << "请输入消息type:\n";
        std::cin >> str1;
        std::cout << "************************"<< str1 <<"\n";
        if(str1 == "buy")
            SendBuyOrderInfo();
        else if(str1 == "sell" )
            SendSellOrderInfo();
        else if(str1 == "cancel"){
            std::cout << "请输入side:";
            str1.clear();
            std::cin >> str1;
            cancelOrderInfo.Side = str1;
            cancelOrderInfo.Symbol = "BTC/RKT";
            std::cout << "请输入price:";
            str1.clear();
            std::cin >> str1;
            cancelOrderInfo.price = str1;
            std::cout << "请输入excelID:";
            str1.clear();
            std::cin >> str1;
            cancelOrderInfo.ExcelID = str1;
            std::cout << "请输入meordid:";
            str1.clear();
            std::cin >> str1;
            cancelOrderInfo.OrderID = str1;
            std::cout << "请输入orderid:";
            str1.clear();
            std::cin >> str1;
            cancelOrderInfo.OrgOrderID = str1;

            SendCancelOrder();
        }else if(str1 == "status"){
            orderStatusReq.Accountid = "1";
            std::cout << "请输入orderid:";
            str1.clear();
            std::cin >> str1;
            orderStatusReq.OrderID = str1;
            orderStatusReq.symbol = "BTC/RKT";
            std::cout << "请输入meordid:";
            str1.clear();
            std::cin >> str1;
            orderStatusReq.meOrderID = str1;
            SendOrderStatReq();
        }else if(str1 == "Retrans"){
            orderRetranReq.Accountid = "1";
            orderRetranReq.symbol = "RKT/EOS";
            std::cout << "请输入orderid:";
            str1.clear();
            std::cin >> str1;
            orderRetranReq.OrderID = str1;
            SendrestranReq();
        }else if(str1 == "History"){
            hiStoryReq.Accountid = "1";
            std::cout << "请输入orderid:";
            str1.clear();
            std::cin >> str1;
            hiStoryReq.OrderID = str1;
            hiStoryReq.symbol = "RKT/EOS";
            std::cout << "请输入meordid:";
            str1.clear();
            std::cin >> str1;
            hiStoryReq.meOrderID = str1;
            SendHistoryReq();
        }
    }
    pause();
}
/*void Applications::run() {
    while (true) {
        std::string str1;
        std::cout << "请输入消息type:";
        std::cin >> str1;
        std::cout << "************************"<< str1 <<"\n";
        if(str1.compare("G1001")  == 0){
            FIX44::HistQuotReq histQuotReq;
            histQuotReq.setField(FIX::Account("1001"));
            histQuotReq.setField(FIX::ClOrdID("500001"));
            histQuotReq.setField(FIX::OrderID("900001"));
            histQuotReq.setField(FIX::Symbol("RKT/ETH"));
            FIX::Session::sendToTarget(histQuotReq, m_sessionID);
        }else if(str1.compare("G1003")  == 0){
            FIX44::OrdRetransReq ordRetransReq;
            ordRetransReq.setField(FIX::Account("1001"));
            ordRetransReq.setField(FIX::OrderID("900001"));
            ordRetransReq.setField(FIX::Symbol("RKT/ETH"));
            FIX::Session::sendToTarget(ordRetransReq, m_sessionID);
        }else if(str1.compare("D")  == 0){
            FIX44::NewOrderSingle newOrderSingle;
            newOrderSingle.setField(FIX::Account("1001"));
            newOrderSingle.setField(FIX::ClOrdID("500001"));
            newOrderSingle.setField(FIX::Symbol("RKT/ETH"));
            newOrderSingle.setField(FIX::OrdType('2'));
            newOrderSingle.setField(FIX::Side('1'));
            newOrderSingle.setField(FIX::Price(0.539));
            newOrderSingle.setField(FIX::OrderQty(1));
            newOrderSingle.setField(FIX::TransactTime(true));
            FIX::Session::sendToTarget(newOrderSingle, m_sessionID);
        }else if(str1.compare("F")  == 0){
            FIX44::OrderCancelRequest orderCancelRequest;
            orderCancelRequest.setField(11, "0");
            orderCancelRequest.setField(41, "500001");
            orderCancelRequest.setField(17, "1");
            orderCancelRequest.setField(37, "900001");
            orderCancelRequest.setField(55, "RKT/ETH");
            orderCancelRequest.setField(54, "1");
            orderCancelRequest.setField(44, "0.539");
            FIX::Session::sendToTarget(orderCancelRequest, m_sessionID);
        }else if(str1.compare("H") == 0){
            FIX44::OrderStatusRequest orderStatusRequest;
            orderStatusRequest.setField(FIX::Account("1001"));
            orderStatusRequest.setField(11, "500001");
            orderStatusRequest.setField(37, "900001");
            orderStatusRequest.setField(55, "RKT/ETH");
            FIX::Session::sendToTarget(orderStatusRequest, m_sessionID);
        }
        sleep(5);
    }
}*/



