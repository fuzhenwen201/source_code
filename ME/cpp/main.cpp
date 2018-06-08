#include <cstdio>
#include <iostream>
#include "ReadInputFile.h"
#include "XRingBuffer.h"
#include <thread>
#include "MacthEngine.h"
#include "TimerClock.h"
#include <functional>
#include <muduo/base/Logging.h>
#include "WriteMarketInfo.h"
#include <stdio.h>
#include <muduo/base/AsyncLogging.h>
#include "./RabbitMQ/tnode.h"
#include "./RabbitMQ/xlogger.h"
#include "ServerStatusMng.h"
#include <thread>
#include "InputDataMng.h"

// void TestIterator(std::multimap<int, int>::iterator* iterTest)
// {
// 	std::multimap<int, int> mulMap;
// 	mulMap.clear();
// 	mulMap.insert(std::make_pair(1, 111));
// 	mulMap.insert(std::make_pair(2, 222));
// 	std::multimap<int, int>::iterator iter = mulMap.insert(std::make_pair(3, 3330));
// 	mulMap.insert(std::make_pair(3, 3331));
// 	mulMap.insert(std::make_pair(4, 444));
// 	mulMap.insert(std::make_pair(5, 555));
// 	std::cout << iter->second << std::endl;
// 
// 	mulMap.erase(1);
// 	std::cout << iter->second << std::endl;
// 
// 	mulMap.insert(std::make_pair(0, 100));
// 	mulMap.erase(5);
// 	std::cout << iter->second << std::endl;
// 
// 	iterTest = &iter;
// 
// 
// }

muduo::AsyncLogging* g_AsynLog = NULL;
snetwork_xservice_tnode::TNode* g_pTNode = NULL;

/*FILE *fp = NULL;*/
// void OutputLog(const char* msg, int len)
// {
// 	size_t n = fwrite(msg, 1, len, fp);
// 	if (n != len)
// 	{
// 		std::cout << "write log file error, actually wirte bytes = " << n << " but should wirtes = " << len << std::endl;
// 	}
// }
void asyncOutput(const char* msg, int len)
{
	g_AsynLog->append(msg, len);
}
void InitLogSys()
{
	(*snetwork_xservice_xflagger::XLogger::GetObject("new_node_test")).StdErr(true).Colour(true).Dir("./logs").BuffDelay(0);
	XINFO("InitLogSys ....");
}

int main(int argc, char *argv[])
{
// 	while (true)
// 	{
// 		printf("hello from Match_enginee_linux_test!\n");
// 
// 	}

// 	std::multimap<int, int>::iterator* iter;
// 	TestIterator(iter);
// 	std::cout << "IterTest =" << (*iter)->second << std::endl;

// 	MemoryPool<STSubmitOrderInfo> memPool;
// 	STSubmitOrderInfo* p1 = memPool.allocate();
// 	p1->chReverse[0] = '1';
// 	memPool.destroy(p1);
// 	memPool.deallocate(p1);
// 	std::map<int, int> mapTest;
// 	mapTest[1] = 111;
// 	mapTest[2] = 222;
// 	mapTest[3] = 333;
// 	mapTest[4] = 444;
// 	mapTest[5] = 555;
// 	std::map<int, int>::reverse_iterator iter = mapTest.rbegin();
// 	while (iter != mapTest.rend())
// 	{
// 		std::cout << "remove:" << iter->first << std::endl;
// 		iter = std::map<int, int>::reverse_iterator(mapTest.erase((++iter).base()));
// 		std::map<int, int>::reverse_iterator iter111 = mapTest.rbegin();
// 		while (iter111 != mapTest.rend())
// 		{
// 			std::cout << "first:" << iter111->first << "second:" << iter111->second << std::endl;
// 			iter111++;
// 		}
// 		
// 
// 	}
	if (argc != 6)
	{
		std::cout << "error,Usage:./MatchingEngine ip port exchange redis_ip redis_port!" << std::endl;
		return 0;
	}

	muduo::Logger::setLogLevel(muduo::Logger::INFO);
	int iRollSize = 100 * 1024 * 1024;
	muduo::AsyncLogging asynLog("match_enginee", iRollSize, 2);
	asynLog.start();
	g_AsynLog = &asynLog;
	muduo::Logger::setOutput(asyncOutput);

	InitLogSys();


//	muduo::Logger::setOutput(OutputLog);
//	muduo::Logger::setFlush(FlushLog);
	
// 	auto p1 = [](int x, int y)->int {x++; return x + y; };
// 	std::cout << p1(2,3) << std::endl;

	LOG_INFO << "start to run match engine progress";
	STTnodeConfig tnodeconfig;
	tnodeconfig.mq_vhost = "admin";
	tnodeconfig.mq_exchange_group = argv[3];
	tnodeconfig.mq_host = argv[1];
	tnodeconfig.mq_port = atoi(argv[2]);
	tnodeconfig.mq_user = "admin";
	tnodeconfig.mq_passwd = "123456";
	snetwork_xservice_tnode::TNode g_TNode(tnodeconfig);
	if (-1 == g_TNode.Init())
	{
		LOG_FATAL <<"tnode init fail , pls check ! \n";
		return -1;
	}

	g_pTNode = &g_TNode;
	
	CMacthEngine matchEnginee;


	//设置定时器,暂时定时器设置在主线程中
	MuduoEventLoop eventLopp;
	CServerStatusMng serverStatus(argv[4], atoi(argv[5]));
	CTimerClock timerClock;
	timerClock.StartTimerByOtherLoop(&eventLopp);
	//设置定时器的回调
	timerClock.SetMatchCallBack(std::bind(&CMacthEngine::SetTimerEvent, &matchEnginee, std::placeholders::_1, std::placeholders::_2));
	timerClock.SetSnapShotCallBack(std::bind(&CMacthEngine::SetTimerEvent,&matchEnginee, std::placeholders::_1, std::placeholders::_2));
	timerClock.SetServerStatusCallBacl(std::bind(&CServerStatusMng::ProcServerStatus, &serverStatus, std::placeholders::_1, std::placeholders::_2));

	//创建输入模块
	CInputDataMng inputDataModule(argv[1], atoi(argv[2]), "admin", argv[3], "admin", "123456");
	inputDataModule.RegisterRingBuf(matchEnginee.GetInputRingBuffer());
	inputDataModule.RegisterSeverStatus(&serverStatus);
	std::thread inputThread(&CInputDataMng::ProcInputData, &inputDataModule, 0);

	//输出信息
	CWriteMarketInfo writeMarket;
	writeMarket.RegisterBuffer(matchEnginee.GetOutputRingBuffer());
	std::thread outPutThread(&CWriteMarketInfo::StartPublishData, &writeMarket, 0);
	//writeMarket.SetRunCpu(0);
	//matchEnginee.SetRunCpu(1);

//	readFile.StartWork();
	matchEnginee.StartWork();
//	writeMarket.StartWork();
	//开始事件循环
	eventLopp.loop();


	inputDataModule.EixtThread();
	inputThread.join();
	writeMarket.ExitWork();
	outPutThread.join();

    return 0;
}