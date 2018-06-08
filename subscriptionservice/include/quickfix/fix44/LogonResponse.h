#ifndef FIX44_LOGONRESPONSE_H
#define FIX44_LOGONRESPONSE_H

#include "Message.h"

namespace FIX44
{

  class LogonResponse : public Message
  {
  public:
      LogonResponse() : Message(MsgType()) {}
      LogonResponse(const FIX::Message& m) : Message(m) {}
      LogonResponse(const Message& m) : Message(m) {}
      LogonResponse(const LogonResponse& m) : Message(m) {}
    static FIX::MsgType MsgType() { return FIX::MsgType("G1000"); }

    FIELD_SET(*this, FIX::EncryptMethod);
    FIELD_SET(*this, FIX::HeartBtInt);
    FIELD_SET(*this, FIX::NextExpectedMsgSeqNum);
    FIELD_SET(*this, FIX::NormalMin);
    FIELD_SET(*this, FIX::NoRelatedSym);
    FIELD_SET(*this, FIX::Symbol);
  };

}

#endif
