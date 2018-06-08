#ifndef _CMSGHANDLE_H_
#define _CMSGHANDLE_H_

#include <map>
#include <unistd.h>
#include <string.h>
#include <stdexcept>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/read_until.hpp> 

#include "worknew.h"
#include "redis.h"

using namespace std;


class CMsghandle : public CDispatch
{
	public:
		CMsghandle();
		~CMsghandle();

		int do_work(std::string strApi,  char* src, char* dest, int& nRoute, std::string strToken, CRedis& pRds,char* szSrcId,stCrmApi& CrmConfig);

		class CBusiness m_Cbusy;
};

#endif
