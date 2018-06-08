#include "../../../include/msgopr.h"

int CIpcMsg::IpcCreMsg(key_t msgkey)
{
	//创建消息队列
	int iRet = 0;
	iRet = msgget(msgkey, IPC_CREAT|0666) ;
	if(iRet < 0)
	{
		LOG_ERR("创建消息队列失败key[%d]!errno[%d]",msgkey, errno);
		return iRet;
	}
	//iRet<0 ? LOG_ERR("创建消息队列失败key[%d]!errno[%d]",msgkey, errno) : m_MsgId = iRet;
	m_MsgId = iRet;
	LOG_INFO("创建消息队列成功,msgkey[%d]msgid[%d]",msgkey,m_MsgId); 
	return iRet;
}

int CIpcMsg::IpcDelMsg()
{
	//删除消息队列
	int  iRet;
	LOG_INFO("MsgId[%d]", m_MsgId);
	if(m_MsgId > 0)
	{
		iRet = msgctl(m_MsgId, IPC_RMID, 0);
		if(iRet < 0)
		{
			LOG_INFO("删除消息队列失败!errno[%d]", errno);
		}
		else
		{
			LOG_INFO("删除消息队列成功[%d]", m_MsgId);
			m_MsgId = 0;
		}
	}
}

int CIpcMsg::IpcRcvMsg(IpcMsgBuf *buf, int len)
{
	//接收消息
	int iRet;
	//iRet = msgrcv(m_MsgId, buf->mtext, len, buf->mtype, 0);
	iRet = msgrcv(m_MsgId, (struct msgbuf *)buf, len, buf->mtype, 0);
	if(iRet < 0) 
		LOG_INFO("接收消息失败!errno[%d]", errno);
#if 0
	iRet < 0 ? 
		LOG_INFO("接收消息失败!errno[%d]", errno): 
				LOG_INFO("接收消息成功[%s]",buf->mtext);
#endif
	return iRet;
}

int CIpcMsg::IpcSndMsg(IpcMsgBuf buf, int len)
{
	//向消息队列写入消息
	int iRet;
	int cnt = 0;
	iRet = msgsnd(m_MsgId, (struct msgbuf *)&buf, len, 0);
	if(iRet < 0) 
		LOG_ERR("写入消息失败!msgid[%ld],errno[%d]", m_MsgId,errno); 
#if 0
	iRet < 0 ? 
		LOG_INFO("写入消息失败!errno[%d]", errno): 
				LOG_INFO("写入消息成功[%s]",buf.mtext);
#endif
	return iRet;
}

int CIpcMsg::GetNumMsg()
{
	int  iRet;
	struct msqid_ds msgds;
	memset(&msgds, 0x00, sizeof(&msgds));
	iRet = msgctl(m_MsgId, IPC_STAT, &msgds);
	if(iRet == -1)
	{
		LOG_ERR("获取消息队列数量失败!");
		return -1;
	}

	return msgds.msg_qnum;
}

CIpcMsg::CIpcMsg()
{
	m_MsgId = 0;
	m_MaxMsg = 1000;
	m_MaxMsgLen = 1024;
}

CIpcMsg::CIpcMsg(int maxmsg, int msglen)
{
	m_MsgId = 0;
	m_MaxMsg = maxmsg;
	m_MaxMsgLen = msglen;
}

#if 0
CIpcMsg::CIpcMsg(key_t msgkey, int maxmsg, int msglen):
		m_Key(msgkey),m_MaxMsg(maxmsg),m_MaxMsgLen(msglen)
{
	int m_MsgId = 0;
	memset(&m_SndMsgBuf, 0x00, sizeof(m_SndMsgBuf));
	LOG_INFO("KEY[%d], MAXMSG[%d], MAXLEN[%d]", m_Key, maxmsg, msglen);
}
#endif

CIpcMsg::~CIpcMsg()
{
}
