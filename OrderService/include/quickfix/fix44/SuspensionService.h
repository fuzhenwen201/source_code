//
// Created by kevin on 3/1/18.
//

#ifndef QUICKFIX_SUSPENSIONSERVICE_H
#define QUICKFIX_SUSPENSIONSERVICE_H

#include "Message.h"

namespace FIX44
{

    class SuspenService : public Message
    {
    public:
        SuspenService() : Message(MsgType()) {}
        SuspenService(const FIX::Message& m) : Message(m) {}
        SuspenService(const Message& m) : Message(m) {}
        SuspenService(const SuspenService& m) : Message(m) {}
        static FIX::MsgType MsgType() { return FIX::MsgType("G1005"); }

        FIELD_SET(*this, FIX::NoRelatedSym);
        FIELD_SET(*this, FIX::Symbol);
        FIELD_SET(*this, FIX::Text);
    };

    class HistQuotReq : public Message
    {
    public:
        HistQuotReq() : Message(MsgType()) {}
        HistQuotReq(const FIX::Message& m) : Message(m) {}
        HistQuotReq(const Message& m) : Message(m) {}
        HistQuotReq(const HistQuotReq& m) : Message(m) {}
        static FIX::MsgType MsgType() { return FIX::MsgType("G1001"); }

        FIELD_SET(*this, FIX::Account);
        FIELD_SET(*this, FIX::ClOrdID);
        FIELD_SET(*this, FIX::OrderID);
        FIELD_SET(*this, FIX::Symbol);
    };

    class HistQuotResp : public Message
    {
    public:
        HistQuotResp() : Message(MsgType()) {}
        HistQuotResp(const FIX::Message& m) : Message(m) {}
        HistQuotResp(const Message& m) : Message(m) {}
        HistQuotResp(const HistQuotResp& m) : Message(m) {}
        static FIX::MsgType MsgType() { return FIX::MsgType("G1002"); }

        FIELD_SET(*this, FIX::ClOrdID);
        FIELD_SET(*this, FIX::OrdStatus);
        FIELD_SET(*this, FIX::Side);
        FIELD_SET(*this, FIX::Symbol);
        FIELD_SET(*this, FIX::Price);
        FIELD_SET(*this, FIX::OrderQty);
        FIELD_SET(*this, FIX::AvgPx);
        FIELD_SET(*this, FIX::CumQty);
        FIELD_SET(*this, FIX::TransactTime);
    };

    class OrdRetransReq : public Message
    {
    public:
        OrdRetransReq() : Message(MsgType()) {}
        OrdRetransReq(const FIX::Message& m) : Message(m) {}
        OrdRetransReq(const Message& m) : Message(m) {}
        OrdRetransReq(const OrdRetransReq& m) : Message(m) {}
        static FIX::MsgType MsgType() { return FIX::MsgType("G1003"); }

        FIELD_SET(*this, FIX::Account);
        FIELD_SET(*this, FIX::OrderID);
        FIELD_SET(*this, FIX::Symbol);
    };

    class OrdRetransResp : public Message
    {
    public:
        OrdRetransResp() : Message(MsgType()) {}
        OrdRetransResp(const FIX::Message& m) : Message(m) {}
        OrdRetransResp(const Message& m) : Message(m) {}
        OrdRetransResp(const OrdRetransResp& m) : Message(m) {}
        static FIX::MsgType MsgType() { return FIX::MsgType("G1004"); }

        FIELD_SET(*this, FIX::Account);
        FIELD_SET(*this, FIX::ExecType);
        FIELD_SET(*this, FIX::Symbol);
        FIELD_SET(*this, FIX::OrdStatus);
        FIELD_SET(*this, FIX::Side);
        FIELD_SET(*this, FIX::ClOrdID);
        FIELD_SET(*this, FIX::OrderQty);
        FIELD_SET(*this, FIX::Price);
        FIELD_SET(*this, FIX::TransactTime);
    };

}

#endif //QUICKFIX_SUSPENSIONSERVICE_H
