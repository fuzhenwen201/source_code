/*************************************************************************
	> File Name: main.cc
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 18 Jan 2018 02:13:07 PM CST
 ************************************************************************/

#include<iostream>
#include"mysqlop.h"
#include"redisop.h"
#include<unistd.h>

int main(int argc, char* argv[]) {
#if 1
	MySqlDB db;
	db.Open();
	db.SelectDB("test");
	MySqlRecordSet rs = db.QuerySql("select now() as tm from dual");
	std::cout<<rs.GetFieldByName(0, "tm")<<std::endl;
	std::cout<<rs.GetFieldByID(0, 0)<<std::endl;
	db.Close();
#endif

	CRedis redis("localhost", 6379, 0, "hugo123456");
	std::shared_ptr<redisReply> ret = redis.Excute("set keyy hello");
	std::cout<<ret.get()->str<<std::endl;
/*	std::shared_ptr<redisReply> retnx = redis.Excute("SET key 99579984251518977 PX 10000 NX");
	std::cout<<retnx.get()->str<<retnx.get()->integer<<std::endl;
	CRedisLock lk("key", "value");
	bool r = redis.Lock(lk);
	if (r) {
		std::cout<<"lock success\n";
		sleep(20);
	} else {
		std::cout<<"lock failure\n";
	}
*/
	redis.SelectDB(4);
	ret = redis.Excute("set hello keyy");
	//ret = redis.Excute("Get %s", "hello");
	std::cout<<ret.get()->str<<std::endl;
	
	redis.SelectDB(0);
	ret = redis.Excute("Get %s", "hello");
	std::cout<<ret.get()->str<<std::endl;
	return 0;
}

