#include "../../include/HttpCliBusiness.h"



void signal_handler(int sig) {
	switch (sig) {
		case SIGTERM:
		case SIGHUP:
		case SIGQUIT:
		case SIGINT:
			break;
	}
}
void timeout(int signo)
{
	LOG_INFO("time out");
}
int main(int argc, char* argv[])
{

	string strResponse = "";
	char* szIp = NULL;
	char* szPort = NULL;
	int   nRet= 0;
	char* tmp = NULL;
	char* szSndkey = NULL;
	char* szSndMax = NULL;
	char* szSynSndkey = NULL;
	char* szSynSndMax = NULL;
	char* szRcvkey = NULL;
	char* szRcvMax = NULL;
	//char* g_Url_Crm_My;

	char* szSndSynkey = NULL;
	char* szSndSynMax = NULL;

	if(argc<2)
		return FAILE;

	InitLog("httpclient", argv[1]);
	LOG_INFO("httpclient[%s][%s]", argv[0], argv[1]);


	char szCmdReq[CMD_LEN+1];

	key_t msgSndkey ;
	key_t msgSynSndkey ;
	key_t msgRcvkey ;

	int len = 0;
	IpcMsgBuf buf;
	memset(&buf, 0x00, sizeof(buf));

	memset(szCmdReq, 0x00, sizeof(szCmdReq));

	buf.mtype = 0;

	 


	M_XRETURN(((szSndMax = getenv("MAX_MSGC"))==NULL),
			false, LOG_ERR("get msq num err!"));
	M_XRETURN(((szSndkey = getenv("MSG_HTTPCLIENT_IPC_BUSSND_KEY"))==NULL), 
			false, LOG_ERR("get msq id err!"));
	sscanf(szSndkey, "%x", &msgSndkey);
	CIpcMsg vcSndMsg(atoi(szSndMax), 10240);
	vcSndMsg.IpcCreMsg(msgSndkey);
	LOG_INFO("消息队列ID[%s]",szSndkey);
	LOG_INFO("消息队列key[%s]",szSndMax);

	M_XRETURN(((szRcvMax = getenv("MAX_MSGE"))==NULL),
			false, LOG_ERR("get msq num err!"));
	M_XRETURN(((szRcvkey = getenv("MSG_HTTPCLIENT_IPC_BUSRCV_KEY"))==NULL), 
			false, LOG_ERR("get msq id err!"));
	sscanf (szRcvkey, "%x", &msgRcvkey);
	CIpcMsg vcRcvMsg(atoi(szRcvMax), 10240);
	vcRcvMsg.IpcCreMsg(msgRcvkey);
	LOG_INFO("消息队列ID[%s]",szRcvkey);
	LOG_INFO("消息队列key[%s]",szRcvMax);

	M_XRETURN(((szSynSndMax = getenv("MAX_MSGB"))==NULL),
			false, LOG_ERR("get msq num err!"));
	M_XRETURN(((szSynSndkey = getenv("MSG_HTTPCLIENT_IPC_BUSSYN_KEY"))==NULL), 
			false, LOG_ERR("get msq id err!"));

	LOG_INFO("消息队列ID[%s]",szSynSndkey);
	LOG_INFO("消息队列key[%s]",szSynSndMax);
	sscanf (szSynSndkey, "%x", &msgSynSndkey);
	LOG_INFO("消息队列标识[%d]",msgSynSndkey);

	CIpcMsg vcSynSndMsg(atoi(szSynSndMax), 10240);
	vcSynSndMsg.IpcCreMsg(msgSynSndkey);


	sigset(SIGALRM, timeout);

	LOG_INFO("start to send req");
	signal(SIGHUP, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

	char Url[128] = {0};
	char szLang[80] = {0};
	char SndTag[2048] = { 0 };
	char szMsgType[MSG_TYPE_LEN + 1] = {0};
	//char* sPos = NULL;
	//char* ePos = NULL;
	int  nLen = 0;
	int  nFlag = 0;
	//int  nRoute = 0;
	MsgHead msghead;

	HttpCliBusiness client;
	client.InitHttpClient();

	while(1)
	{
		nFlag = 0;
		 
		LOG_INFO("start rcv msg");
		memset(buf.mtext, 0x00, BUFSZ);
		buf.mtype = 0;
		strResponse = "";
		memset(Url,0x00,sizeof(Url));
		//memset(gLang, 0x00, sizeof(gLang));
		if ((nRet = vcRcvMsg.IpcRcvMsg(&buf, BUFSZ)) <= 0)
		{
			sleep(1);
			LOG_INFO("RcvMsgErr[%d]",nRet);
			continue;
		}
		 
		//LOG_INFO("len url[%d], Sndtag[%d]", sizeof(Url), sizeof(SndTag));
		//memset(Url, 0x00, sizeof(Url));
		//memset(SndTag, 0x00, sizeof(SndTag));
		memset(szMsgType, 0x00, sizeof(szMsgType));
		memcpy(szMsgType, buf.mtext, MSG_TYPE_LEN);
		memcpy((char*)&msghead, buf.mtext, sizeof(MsgHead));

		//GetTagVal(buf.mtext, (char *)TAG_LANGUAGE, gLang);
		//LOG_INFO("langue[%s]", gLang);
		 

		int nRoute = client.RequestHttpCli(buf.mtext,Url, strResponse);
		buf.mtype = strtol(szMsgType,NULL,10);
		LOG_INFO("szMsgType[%s],type[%ld]",szMsgType,buf.mtype);
	
		memset(buf.mtext, 0x00, BUFSZ);
		memcpy(buf.mtext, (char* )&msghead,sizeof(MsgHead));
		buf.mtext[MAC_LEN+CRM_USER_LEN+CRM_PWD_LEN+MT4_USER_LEN+MT4_PWD_LEN] = '9';
		strcat(buf.mtext, "|");
		strcat(buf.mtext, Url);
		strcat(buf.mtext, "|");
		if(strResponse.size() >= BUFSZ)
		{
			LOG_INFO("data is more long");
			continue;
		}
		strcat(buf.mtext, strResponse.c_str());
		//int len = (BUFSZ-1) < strResponse.size() ? (BUFSZ-1) : strResponse.size();
		//strncat(buf.mtext,strResponse.c_str(),len);
		LOG_INFO("Route[%d] data[%s]", nRoute, buf.mtext);
		if(nRoute == 2)
		{
			M_XRETURN((vcSynSndMsg.IpcSndMsg(buf, strlen(buf.mtext)) < 0), 
					false, LOG_ERR("Req buf err!"));
		}
		else 
		{
			M_XRETURN((vcSndMsg.IpcSndMsg(buf, strlen(buf.mtext)) < 0), 
					false, LOG_ERR("Req buf err!"));
		}
		 

		LOG_INFO("finish req");
	}
	return 0 ;
}
