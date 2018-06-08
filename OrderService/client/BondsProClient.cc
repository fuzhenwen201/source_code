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

static FIX::SessionID m_sessionID;
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
        message.setField(FIX::HeartBtInt(25));
        message.setField(FIX::EncryptMethod(0));
    }
    std::cout << "1 ADMIN OUT: " << message << std::endl;
}

void Applications::fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionID)
    throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {
    std::cout << "2 ADMIN IN: " << message << std::endl;
}

void Applications::onLogon(const FIX::SessionID& sessionID) {
    std::cout << "3 Logon - " << sessionID << std::endl;
}

void Applications::fromApp(const FIX::Message& message, const FIX::SessionID& sessionID)
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType ) {
    std::cout << "4 IN: " << message << std::endl;
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
    std::cout << FIELD_GET_REF( message.getHeader(), MsgType) << " \n";
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

void Applications::run() {
    while (true) {
        std::string str1;
        std::cout << "请输入消息type:";
        std::cin >> str1;
        std::cout << "************************"<< str1 <<"\n";
        if(str1.compare("G1001")  == 0){
            FIX44::HistQuotReq histQuotReq;
            histQuotReq.setField(FIX::Account("4"));
            histQuotReq.setField(FIX::ClOrdID("500001"));
            histQuotReq.setField(FIX::OrderID("900001"));
            histQuotReq.setField(FIX::Symbol("RKT/EOS"));
            FIX::Session::sendToTarget(histQuotReq, m_sessionID);
        }else if(str1.compare("G1003")  == 0){
            FIX44::OrdRetransReq ordRetransReq;
            ordRetransReq.setField(FIX::Account("4"));
            ordRetransReq.setField(FIX::OrderID("900001"));
            ordRetransReq.setField(FIX::Symbol("RKT/EOS"));
            FIX::Session::sendToTarget(ordRetransReq, m_sessionID);
        }else if(str1.compare("D")  == 0){
            FIX44::NewOrderSingle newOrderSingle;
            newOrderSingle.setField(FIX::Account("9"));
            newOrderSingle.setField(FIX::ClOrdID("500001"));
            newOrderSingle.setField(FIX::Symbol("RKT/EOS"));
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
            orderCancelRequest.setField(55, "RKT/EOS");
            orderCancelRequest.setField(54, "1");
            orderCancelRequest.setField(44, "0.539");
            FIX::Session::sendToTarget(orderCancelRequest, m_sessionID);
        }else if(str1.compare("H") == 0){
            FIX44::OrderStatusRequest orderStatusRequest;
            orderStatusRequest.setField(FIX::Account("4"));
            orderStatusRequest.setField(11, "500001");
            orderStatusRequest.setField(37, "900001");
            orderStatusRequest.setField(55, "RKT/EOS");
            FIX::Session::sendToTarget(orderStatusRequest, m_sessionID);
        }
        sleep(5);
    }
}



