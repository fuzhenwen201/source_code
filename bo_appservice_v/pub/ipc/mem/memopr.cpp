#include "../../../include/memopr.h"

int CIpcMem::Delete(int shmid)
{
	if (shmctl(shmid, IPC_RMID, 0) == -1)
	{
		LOG_ERR("static delete share memory error shmid[%d],errno[%d]", shmid, errno);
		return -1;
	}

	return 0;
}

CIpcMem::CIpcMem()
{
	shmId = 0;
	bzero(&shMem, sizeof(shMem));
}

CIpcMem::~CIpcMem()
{
}

int CIpcMem::Create(key_t shmkey)
{
	// 创建共享内存
	shmId = shmget(shmkey, sizeof(IpcMemBuf), 0666 | IPC_CREAT);
	if (shmId == -1)
	{
		LOG_ERR("create shm error with key[%d]", shmkey);
		return -1;
	}
	// 将共享内存连接到当前进程的地址空间
	shMem = (pIpcMemBuf)shmat(shmId, NULL, 0);
	if (shMem == (void*)-1)
	{
		LOG_ERR("create shm shmat error key[%d]", shmkey);
		Delete();
		return -2;
	}
	
	LOG_INFO("create a share memory successful,shmId[%d],shmkey[%d]", shmId, shmkey);

	return 0;
}

pIpcMemBuf CIpcMem::GetBuf()
{
	return shMem;
}

int CIpcMem::GetShmid()
{
	return shmId;
}

int CIpcMem::Detach()
{
	if(shmdt(shMem) == -1)
	{
		LOG_ERR("detach share memory error shmid[%d],errno[%d]", shmId, errno);
		return -1;
	}
	return 0;
}

int CIpcMem::Delete()
{
	if (shmctl(shmId, IPC_RMID, 0) == -1)
	{
		LOG_ERR("delete share memory error shmid[%d],errno[%d]", shmId,errno);
		return -1;
	}

	return 0;
}
