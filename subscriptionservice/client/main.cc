/*************************************************************************
    > File Name: main.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Thu 11 Jan 2018 12:19:37 PM CST
 ************************************************************************/
#include "quickfix/FileStore.h"
#include "quickfix/FileLog.h"
#include "quickfix/ThreadedSocketInitiator.h"
#include "BondsProClient.h"

int main(int argc, char** argv) {

    try {
        FIX::SessionSettings settings("BondsProClient.cfg");

        Applications application(settings);
        FIX::FileStoreFactory storeFactory(settings);
        FIX::FileLogFactory logFactory(settings);
        FIX::ThreadedSocketInitiator initiator(application, storeFactory, settings, logFactory);

        initiator.start();

        application.run();
        initiator.stop();

        return 0;
    }
    catch (std::exception & e) {
        std::cout << e.what();
        return 1;
    }
}
