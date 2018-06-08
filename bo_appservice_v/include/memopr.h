#ifndef _IPCMEMOPR_H_
#define _IPCMEMOPR_H_ 

#include "log.h"
#include "const.h"
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

#define  SHMBUFSZ 5242880 // 1024 *1024 * 5 = 5MB

typedef struct MemBuf
{
	unsigned int msize;	// 实际的数据长度
	char mtext[SHMBUFSZ+1]; // buffer长度
}IpcMemBuf, *pIpcMemBuf;

class CIpcMem
{
public:
	static int Delete(int shmid);

public:
	CIpcMem();
	~CIpcMem();

public:
	int Create(key_t shmkey);
	pIpcMemBuf GetBuf();
	int GetShmid();
	int Detach();
	int Delete();

private:
	int shmId;
	pIpcMemBuf shMem;
};

#endif
