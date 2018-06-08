//
// Created by kevin on 7/6/17.
//

#ifndef MONITOR_BRIDGE_H
#define MONITOR_BRIDGE_H
#include <gflags/gflags.h>
#include <assert.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <sys/socket.h>

#include "tnodeconfig_v0.h"
#include "tnode.h"
#include "public.h"

extern bool g_connect_flags;
class MonitorBridge {
    public:
    MonitorBridge();
        virtual ~MonitorBridge();
        void ParseCommandLine(int* argc, char*** argv, bool cv);
        const std::string& Getconfig(const std::string& str);
};

class MonitorConsumer : public snetwork_xservice_tnode::TNodeConsumer{
    public:
        MonitorConsumer(const std::string &instance_id, snetwork_xservice_tnode::TNodeChannel &channel);
        virtual ~MonitorConsumer();

        std::string GetEventName();
        int32_t ConnectService(const char* ip, uint16_t port);
        uint32_t EventHandle(u_int16_t tag, char *event, uint32_t eventlen);

        uint32_t ServiceHandle(u_int16_t tag, char *reqbody, uint32_t reqlen, snetwork_xservice_tnode::TNodeDataSend send);

    private:
        std::string instance_id_;
        snetwork_xservice_tnode::TNodeChannel channel_;
};

#endif //MONITOR_BRIDGE_H
