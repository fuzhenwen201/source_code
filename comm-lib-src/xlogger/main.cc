#include <iostream>
#include "xlogger.h"
#include<glog/logging.h>
#include<thread>

using namespace std;

XLOG* g_log = NULL;

void f1(void) 
{
	do {
	XINFO("=======thread1111111111\n");
	}while(1);
}

void f2(void) 
{
	do {
	XINFO("-------thread2222222222\n");
	}while(1);
}

int main(int argc, char* argv[]) {

	//XLOGGER(argv[0]).StdErr(true).Colour(true).Dir("./");
	XLOGGER(argv[0]).StdErr(false).Colour(true).Dir("./").BuffDelay(0);

    //XLOGGER(argv[0]).StdErr(false).Colour(true).Dir("/home/leaker/tmp/logger/log");
    //g_log = XLOGGER(argv[0]);
    //g_log.StdErr(true);
	//g_log.Colour(true);
//	g_log->Dir("/home/leaker/Desktop/project/xlogger/log");

    //google::InitGoogleLogging(argv[0]);
    //FLAGS_log_dir = "/home/derek/TradingPlatform/xlogger/";
    //LOG(INFO)<<"helloworld"<<std::endl;

	int i=0;
	while(1){
		if(i%20==0)
			sleep(1);
	
		XERROR("EEEEEE%d\n", i);
		i++;
	}

    XINFO("I am info =====");
	std::string str = "HELLO WORLD!";
    XINFO("======%d %s %c", 1000, "test", 'A');
    XINFO(str);

	//std::thread t1(f1);
	//std::thread t2(f2);
	//t1.join();
	//t2.join();

    XWARNING("helloworld");
    XERROR("eerere");
    XWARNING("helloworld%s\n", "sldjfsklllllllllllllll");
    XERROR("eerere%d%s", 90000, "w904856043");

    //XLOGGER("")->Verbose(99);
    XLOGGER("").Verbose(200);

    XVINFO(100, "helloworld111111111111111");
    XVWARNING(200, "helloworld");
    XVERROR(300, "eerere");

	LOG(INFO)<<"xdsl======dkjl";
	//g_log->FreeObject();
    FREEXLOGGER;

    return 0;
}

