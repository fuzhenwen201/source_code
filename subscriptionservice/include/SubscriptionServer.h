/*************************************************************************
> File Name: SubscriptionServer.h
> Author: Kevin
> Mail: kevin.fu@fintechstar.cn
> Created Time: Thu 11 Jan 2018 12:25:05 PM CST
************************************************************************/

#ifndef ORDERMATCH_APPLICATION_H
#define ORDERMATCH_APPLICATION_H

#include <list>
#include <iostream>
#include "xlogger.h"
#include "quickfix/Session.h"
#include "quickfix/FieldMap.h"
#include "quickfix/FileStore.h"
#include "quickfix/FileLog.h"
#include "quickfix/SocketAcceptor.h"
#include "quickfix/ThreadedSocketAcceptor.h"
#include <fstream>
#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Utility.h"
#include "quickfix/Mutex.h"
#include "quickfix/fix44/MarketDataSnapshotFullRefresh.h"
#include "quickfix/fix44/Logon.h"
#include "quickfix/fix44/SuspensionService.h"
#include "SubscriptionService.h"

extern std::vector<std::string> g_username;
extern pthread_mutex_t g_usernamelock;

class Applications : public FIX::Application, public FIX::MessageCracker {
    public:
        Applications();
        ~Applications();
        // Application overloads
        void init(const std::string & configFile, STTnodeConfig config);
    private:
        void onCreate( const FIX::SessionID& );
        void onLogon( const FIX::SessionID& sessionID );
        void onLogout( const FIX::SessionID& sessionID );
        void toAdmin( FIX::Message&, const FIX::SessionID& );
        void toApp( FIX::Message&, const FIX::SessionID& )
            throw( FIX::DoNotSend );
        void fromAdmin( const FIX::Message&, const FIX::SessionID& )
            throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon );
        void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
            throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );
        void onMessage ( const FIX44::Logon& message, const FIX::SessionID& );
        void onMessage (const FIX44::HistQuotReq& message, const FIX::SessionID& );
        void onMessage (const FIX44::OrdRetransReq& message, const FIX::SessionID& );
    private:
        FIX::MessageStoreFactory * pMessageStoreFactory_;
        FIX::FileLogFactory * pLogFactory_;
        FIX::SessionSettings * pSessionSettings_;
        FIX::ThreadedSocketAcceptor * pAcceptor_;
};

#endif