#include <iostream>
#include"solution_config.h"
#include"xconfig.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

using namespace std;
using namespace snetwork_xservice_xflagger;

int main(int argc, char* argv[]) {
	XConfig* cfg = new SXConfig;
	XFlagger* flagger = SXFlagger::GetInstance();
	flagger->ParseCommandLine(&argc, &argv, false);
	flagger->FlagXConfig(cfg);
	SXConfig* sxconfig = dynamic_cast<SXConfig*>(flagger->GetConfig());
	if (sxconfig == nullptr) {
		fprintf(stderr, "config error");

		exit(1);
	}
	std::cout<<*dynamic_cast<SXConfig*>(flagger->GetConfig())<<std::endl;
	//const char* a[] ={NULL, "solution.cfg", NULL};
	//char** arr = new char*[2];
	//arr[0] = new char[10]();
	//strcpy(arr[0], "SolutionGateway");
	//arr[1] = new char[40]();
	//strcpy(arr[1], "--flagfile=solution.cfg");
	//int ac = 2;
	//flagger->ParseCommandLine(&argc, &argv, true);
	//flagger->ParseCommandLine(&ac, (char***)&arr, false);
	
#if 0
	//flaager = MySqlXFlagger::GetInstance();
	strcpy(arr[1], "--flagfile=mysqldatabase.cfg");
	XConfig* cfg1 = new MySqlXConfig;
	flagger->ParseCommandLine(&ac, (char***)&arr, false);
	flagger->FlagXConfig(cfg1);
	std::cout<<*dynamic_cast<MySqlXConfig*>(flagger->GetConfig())<<std::endl;
#endif
	delete flagger;
	//delete arr[0];
	//delete arr[1];
	//delete arr;

#if 0

    XFLAGGER.SetVersion("v1.0.10");

    //XFLAGGER.ParseCommandLine(&argc, &argv, true);
    XFLAGGER.ParseCommandLine(&argc, &argv, false);

    XFLAGGER.FlagXConfig(cfg);

    std::cout<<cfg.Pmt1()<<std::endl;
    std::cout<<cfg.Pmt2()<<std::endl;
    std::cout<<cfg.Pmt3()<<std::endl;

    XFREEFLAGGER;

    //XFLAGGER.FlagSNode()
#endif

    return 0;
}
