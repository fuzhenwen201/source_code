//
// Created by alex on 12/29/16.
//

#include <iostream>
#include "tnodelog.h"

using namespace snetwork_xservice_tnode;

static char* GetCurUTCTime() {
    time_t nowtime;
    time(&nowtime);
    struct tm gm_time;

    gmtime_r(&nowtime, &gm_time);

    return ctime(&nowtime);
}

TNodeLog:: TNodeLog(){

}

TNodeLog:: ~TNodeLog(){

}

void TNodeLog::Log(const char* message){
    std::cout << "TNode " << GetCurUTCTime() << " : " << message << std::endl;
}

void TNodeLog::OpLog(const char* message){
    std::cout << "TNode " << GetCurUTCTime() << " : " << message << std::endl;
}

void TNodeLog::ErrLog(const char* message){
    std::cout << "TNode " << GetCurUTCTime() << " : " << message << std::endl;
}