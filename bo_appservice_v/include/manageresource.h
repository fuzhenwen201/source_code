#ifndef _MANAGERESOURCE_H_
#define _MANAGERESOURCE_H_

#include "memopr_boost.h"
#include "log.h"
#include "const.h"
#include "msgopr.h"

//#include "common.h"
#include <boost/asio/deadline_timer.hpp>  
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/asio/read_until.hpp>  
#include <boost/asio/streambuf.hpp>  
#include <boost/asio/write.hpp> 
#include <boost/lexical_cast.hpp> 
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>  
#include <time.h>


void DetectionTimeOut(const boost::system::error_code& e ,boost::asio::deadline_timer* t,void* param);

//static void Run_DeadLineTimer(void* param);
class ManageResource
{
	public:
		ManageResource(key_t key,int type);
		~ManageResource();
		static void Run_DeadlineTimer(void* param);
		int InsertResource(char* apptoken,char* crmtoken,char* crmpwd,int time,int crm_time);
		int DeleteResource(char* apptoken,char* crmtoken);
		int UpdateResource(char* apptoken,char* crmtoken,char* crmpwd,int time,int crm_time);
		CIpcMemBoost* GetIpcMem() { return pIpcMem; };
	protected:
		void InitShareResource(key_t key,int type);
	private:
		CIpcMemBoost *pIpcMem;
		static boost::asio::deadline_timer *timer;
};




#endif
