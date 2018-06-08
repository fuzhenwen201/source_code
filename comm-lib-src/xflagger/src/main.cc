/*************************************************************************
	> File Name: main.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 18 Jan 2018 02:13:07 PM CST
 ************************************************************************/

#include<iostream>
#include"rxconfig.h"

int main(int argc, char* argv[]) {
	RXConfig* cfg = new RXConfig;
	RXFlagger* flagger = RXFlagger::GetInstance();
	flagger->ParseCommandLine(&argc, &argv, true);
	//flagger->FlagConfig(std::move(cfg));
	flagger->FlagConfig(cfg);
	std::cout<<*cfg<<std::endl;

#if 0
	ConnectionPool::GetInstance(dbConfig);

	MySqlDB db;
	db.Open();
	db.SelectDB("test");
	MySqlRecordSet rs = db.QuerySql("select now() as tm from dual");
	std::cout<<rs.GetFieldByName(0, "tm")<<std::endl;
	std::cout<<rs.GetFieldByID(0, 0)<<std::endl;
	db.Close();
#endif

	return 0;
}

