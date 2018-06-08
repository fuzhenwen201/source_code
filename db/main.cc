/*************************************************************************
	> File Name: main.cpp
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Mon 16 Jan 2017 01:45:20 AM PST
 ************************************************************************/
#include<iostream>
#include<cstdlib>
#include<thread>
#include<unistd.h>
#include"mysqlop.h"
//#include"../xlogger/xlogger.h"
#include"xlogger.h"
#include"solution_config.h"

using namespace snetwork_xservice_db;
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
	std::cout<<*sxconfig<<std::endl;
	MySqlDB db;
	db.Open();
	db.SelectDB("test");
	MySqlRecordSet rs = db.QuerySql("select now() as tm from dual");
	std::cout<<rs.GetFieldByName(0, "tm")<<std::endl;
	std::cout<<rs.GetFieldByID(0, 0)<<std::endl;
	db.Close();

	return 0;
}

