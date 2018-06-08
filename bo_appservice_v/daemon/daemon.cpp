#include "../include/daemon.h"

pid_t lStartProcess(Pci& pci)
{
	pid_t    llPid;
	long     lnTime;

	llPid = fork();
	if( llPid == -1 )
	{
		LOG_INFO("fork err");
		return -FAILE;
	}
	else if( llPid > 0 )
	{
		LOG_INFO("start pid[%d]", llPid);
		pci.pid = llPid;
		++pci.nStartCnt;
		return llPid;
	}
	else
	{

		LOG_INFO("argv0[%s]argv1[%s]", pci.argv[0],pci.argv[1]);
		if(execvp(pci.strPath.c_str(), (char**)pci.argv) == FAILE)
		{
			sleep(1);
			LOG_INFO("start[%s]argv2[%s]fail at execvp errno[%d].", pci.strPath.c_str(),pci.argv[1],errno);
		}
		return FAILE;
	}

	return FAILE;

}

void vStartAllProcess()
{
	char *  pFilePath = NULL;
	FILE *fp = NULL;
	char line[2048];
	struct tms  tTMS;

	memset(line, 0x00, sizeof(line));

	if((pFilePath = getenv("DM_FILE_CFG")) == NULL)
	{
		LOG_INFO("get configure err");
		return ;
	}
	if((fp = fopen(pFilePath, "r")) != NULL )
	{
		while(fgets(line, sizeof(line), fp) != NULL)
		{
			if(line[0] == '#' || line[0] == '\n')
			{
				continue;
			}
			//解析进程
			string bin;
			string arg0;
			string arg1;
			Pci pci;
			memset(pci.argv, 0x00, 20*sizeof(char*));

			Json::Value value;
			Json::Reader reader;
			if (reader.parse(line, value))
			{
				M_VRETURN(!(value["procnum"].isInt()), LOG_ERR("get procnum err"));
				M_VRETURN(!(value["times"].isInt()),LOG_ERR("get times err"));
				M_VRETURN(!(value["unittime"].isInt()),LOG_ERR("get unittime err"));
				M_VRETURN(!(value["maxtimes"].isInt()),LOG_ERR("get maxtimes err"));
				M_VRETURN(!(value["bin"].isString()),LOG_ERR("get bin err"));

				pci.strPath = value["bin"].asString();
				pci.nprocnum = value["procnum"].asInt();
				pci.ntimes = value["times"].asInt();
				pci.nunitTime = value["unittime"].asInt();
				pci.nmaxtimes = value["maxtimes"].asInt();
				LOG_INFO("bin[%s],num[%d],ntimis[%d]nunitTime[%d],maxtimes[%d]",
					   	pci.strPath.c_str(), pci.nprocnum, pci.ntimes, pci.nunitTime, pci.nmaxtimes);

				const Json::Value arrayArgs = value["args"];
				for (unsigned int i = 0; i < arrayArgs.size(); i++)
				{
					if(i == 0)
					{
						pci.strArgv[i] = arrayArgs[i].asString();
						pci.argv[i] = (char*)pci.strArgv[i].c_str();
						LOG_INFO("ARGV[%d][%s]",i, pci.argv[i]);
					}
					else
					{
						pci.strArgv[i+1] = arrayArgs[i].asString();
						pci.argv[i+1] = (char*)pci.strArgv[i+1].c_str();
						LOG_INFO("ARGV[%d][%s]",i+1, pci.argv[i+1]);
					}
				}

			}

			for(int n=0; n<pci.nprocnum; n++)
			{
				pci.lnLastTime = time(NULL);
				pci.nSrvSeqId = n;
				pci.nStartCnt = 0;
				pci.strArgv[1] = int2str(pci.nSrvSeqId);
				pci.argv[1] = (char*)pci.strArgv[1].c_str();
				if(FAILE == lStartProcess(pci))
				{
					LOG_INFO("start[%s]faile", pci.strPath.c_str());
				}
				else
				{
					lisproc.push_back(pci);
					LOG_INFO("start[%s] sucess times[%d] time[%ld] seqid[%d]",
							pci.strPath.c_str(), pci.nStartCnt, pci.lnLastTime, pci.nSrvSeqId);
				}
			}
		}
	}
}

void vResetStartCnt()
{
}

void vStopAllProcess()
{
	sigset (SIGCLD, SIG_IGN);
	list<Pci>::iterator it;
	for(it = lisproc.begin(); it != lisproc.end();)
	{
		int llResult = kill((*it).pid, SIGTERM );
		if( llResult == -1 )
		{
			LOG_INFO("stop %ld error, %d.seqid[%d]", (*it).pid, errno,(*it).nSrvSeqId);
		}
		lisproc.erase(it++);
	}
}

#if 0
pPci vFindPci(int pid)
{
	list<Pci>::iterator it;
	for(it = lisproc.begin(); it != lisproc.end();)
	{
		if((*it).pid == pid)
		{
			LOG_INFO("rst pid[%d]", pid);
			return &(*it);
		}
	}
	return NULL;
}
#endif

void vHandleExit(int n)
{
	LOG_INFO("sig[%d]",n);
}

void vHandleChildDeath(int n)
{
	long    llPid;
	int    statu;
	long   lCurTime;
	struct tms  tTMS;

	LOG_INFO("begin.");

	char* args[56] = {0};
	if( (llPid = wait(&statu)) >0 )
	{
		LOG_INFO("exit proc pid[%ld]statu[%d]", llPid, statu);
		sighold( SIGCLD );

		sigset (SIGCLD, SIG_IGN);
		list<Pci>::iterator it;
		for(it = lisproc.begin(); it != lisproc.end(); ++it)
		{
			lCurTime = time(NULL);
			LOG_INFO("pid[%d]lpid[%ld], CutTime[%ld] lasttime[%ld] strcnt[%d]", 
					(*it).pid,  llPid, lCurTime, (*it).lnLastTime, (*it).nStartCnt);
			//if((*it).pid == llPid && (*it).ntimes> (*it).nStartCnt)
			if((*it).pid == llPid )
					//&& lCurTime - (*it).lnLastTime > (*it).nunitTime)
			{
					usleep(1000*100);
					if(FAILE == lStartProcess((*it)))
					{
						LOG_INFO("rst[%s]arg1[%s]arg2[%s]faile",
								(*it).argv[0],(*it).argv[1]);
					}
					else
					{
						(*it).lnLastTime = lCurTime;
						LOG_INFO("rst[%s]sucess", (*it).argv[0]);
					}
				break;
			}
		}
		sigrelse( SIGCLD );
	}
	sigset(SIGCLD,vHandleChildDeath);
	LOG_INFO("end");
}

int main(int argc, char **argv)
{

	if(argc<2)
		return FAILE;

	if( FAILE == InitLog("daemon", argv[1]))
	{   
		return FAILE;
	}   
	LOG_INFO("[%s][%s]", argv[0], argv[1]);

	//m_interval = 1;
	sighold( SIGCLD );
	sigset( SIGCLD, vHandleChildDeath );
	sighold( SIGTERM );
	sigset( SIGTERM, vHandleExit );
	sigrelse(SIGTERM);

	/* start all processes */
	sighold(SIGCLD);
	vStartAllProcess();
	sigrelse(SIGCLD);
	while(1)
	{
		sleep(120);
	}
	return 0;
}
