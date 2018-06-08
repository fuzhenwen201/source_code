#ifndef FIX44_ORDERSTATUS_H
#define FIX44_ORDERSTATUS_H

#include "Message.h"

namespace FIX44
{

  class OrderStatus : public Message
  {
  public:
      OrderStatus() : Message(MsgType()) {}
      OrderStatus(const FIX::Message& m) : Message(m) {}
      OrderStatus(const Message& m) : Message(m) {}
      OrderStatus(const OrderStatus& m) : Message(m) {}
    static FIX::MsgType MsgType() { return FIX::MsgType("DF"); }

    FIELD_SET(*this, FIX::BidOrderID);
    FIELD_SET(*this, FIX::AskOrderID);
    FIELD_SET(*this, FIX::BidPlatformID);
    FIELD_SET(*this, FIX::AskPlatformID);
    FIELD_SET(*this, FIX::Prices);
    FIELD_SET(*this, FIX::Number);
    FIELD_SET(*this, FIX::ExchType);
  };

}

#endif
