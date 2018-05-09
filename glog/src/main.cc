#include "guuid.h"
#include "selfconfig.h"

using namespace snetwork_xservice_config;

int main(int argc,char* argv[]) {
    auto *selfconfig = new SelfConfig;
    selfconfig->ParseCommandLine(&argc, &argv, false);
    selfconfig->InitGoogleLogging(argv);
    FLAGS_log_dir = selfconfig->GetStringconfig("log_dir");
    FLAGS_logtostderr = selfconfig->Getboolconfig("logtostderr");
    FLAGS_colorlogtostderr = selfconfig->Getboolconfig("colorlogtostderr");
    FLAGS_logbufsecs = selfconfig->Getintconfig("logbufsecs");
    FLAGS_max_log_size = selfconfig->Getintconfig("maxlogsize");

    auto u_id_ptr = new utils::unique_id_t();
    u_id_ptr->set_epoch(uint64_t(0));
    u_id_ptr->set_machine(int32_t(0));

    LOG(INFO) << u_id_ptr->generate();
    LOG(INFO) << "info message";  
    LOG(WARNING) <<"warning message";
    LOG(ERROR) << "error message";
    return 0;  
} 
