
#include "monitorbridge.h"

int main(int argc, char* argv[]) {

    MonitorBridge *monitorbridge = new MonitorBridge();
    monitorbridge->ParseCommandLine(&argc, &argv, false);
    XLOGGER(monitorbridge->Getconfig("loggerfile").c_str())
        .StdErr(false)
        .Colour(true)
        .Dir(monitorbridge->Getconfig("loggerdir").c_str());

    snetwork_xservice_tnode::TNodeConfigV0 servercfg;

    servercfg
        .VHost(monitorbridge->Getconfig("mqvhost").c_str())
        .Exchange(monitorbridge->Getconfig("mqExchange").c_str())
        .ExchangeGroup(monitorbridge->Getconfig("mqExchangeGroup").c_str())
        .Address(monitorbridge->Getconfig("mqAddress").c_str())
        .Port(monitorbridge->Getconfig("mqPort").c_str())
        .User(monitorbridge->Getconfig("mqUser").c_str())
        .Password(monitorbridge->Getconfig("mqPassword").c_str());

    snetwork_xservice_tnode::TNode tnode_;

    tnode_.Config(servercfg);
    tnode_.Bind(1);
    tnode_.Run();

    snetwork_xservice_tnode::TNodeChannel channel = tnode_.GetChannel(0);

    MonitorConsumer *monitor_consumer_ = new MonitorConsumer("SGD", channel);

    channel.CreateGroup("MONITORD");
    channel.AddConsumerToGroup("MONITORD", monitor_consumer_);
    do {
        if (0 == strncmp(monitorbridge->Getconfig("sgdip").c_str(), "localhost", 10)) {
            monitor_consumer_->ConnectService("127.0.0.1", atoi(monitorbridge->Getconfig("sgdport").c_str()));
        } else {
            monitor_consumer_->ConnectService(monitorbridge->Getconfig("sgdip").c_str(),
                                              atoi(monitorbridge->Getconfig("sgdport").c_str()));
        }
    }while(g_connect_flags == false);

    return 0;
}
