/*************************************************************************
    > File Name: BondsProClient.h
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Thu 11 Jan 2018 12:24:49 PM CST
 ************************************************************************/

#ifndef TRADECLIENT_APPLICATION_H
#define TRADECLIENT_APPLICATION_H

#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Mutex.h"

#include "quickfix/fix44/NewOrderSingle.h"
#include "quickfix/fix44/ExecutionReport.h"
#include "quickfix/fix44/OrderCancelRequest.h"
#include "quickfix/fix44/OrderCancelReject.h"
#include "quickfix/fix44/OrderCancelReplaceRequest.h"
#include "quickfix/fix44/MarketDataRequest.h"
#include "quickfix/fix44/MarketDataSnapshotFullRefresh.h"
#include "quickfix/fix44/SuspensionService.h"

#include <queue>

class Applications : public FIX::Application, public FIX::MessageCracker {
public:
    Applications(const FIX::SessionSettings&);
    void run();

private:
    void onCreate(const FIX::SessionID& );
    void onLogon( const FIX::SessionID& sessionID );
    void onLogout( const FIX::SessionID& sessionID );
    void toAdmin( FIX::Message&, const FIX::SessionID& );
    void toApp( FIX::Message&, const FIX::SessionID& )
        throw( FIX::DoNotSend );
    void fromAdmin( const FIX::Message&, const FIX::SessionID& )
        throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon );
    void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
        throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

    void onMessage( const FIX44::MarketDataSnapshotFullRefresh&, const FIX::SessionID& );
    void onMessage ( const FIX44::ExecutionReport & message, const FIX::SessionID& );
    void onMessage ( const FIX44::HistQuotResp & message, const FIX::SessionID& );
    void onMessage ( const FIX44::OrdRetransResp & message, const FIX::SessionID& );
    void onMessage ( const FIX44::SuspenService & message, const FIX::SessionID& );
    const FIX::SessionSettings& settings_;
};

#endif
