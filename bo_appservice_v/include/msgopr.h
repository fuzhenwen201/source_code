#ifndef _IPCOPR_H_
#define _IPCOPR_H_ 

#include "log.h"
#include "const.h"
#include <errno.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

typedef struct MsgBuf
{
	unsigned long mtype;
	char mtext[BUFSZ];
}IpcMsgBuf, *pIpcMsgBuf;

class CIpcMsg
{

public:
	CIpcMsg();
	CIpcMsg(int maxmsg, int msglen);
	~CIpcMsg();

public:
	int GetNumMsg();
	int IpcDelMsg();
	int IpcCreMsg(key_t msgkey);
	int IpcRcvMsg(IpcMsgBuf *buf, int len);
	int IpcSndMsg(IpcMsgBuf buf, int len);

public:
	key_t             m_Key;
	int               m_MsgId;
	int               m_MaxMsg;  //最大消息数
	int               m_MaxMsgLen; //消息最大长度

	//IpcMsgBuf         m_SndMsgBuf;
	//IpcMsgBuf         m_RcvMsgBuf;
};

#endif
