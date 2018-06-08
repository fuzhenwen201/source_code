#ifndef _ONLINESERVICE_H_
#define _ONLINESERVICE_H_

#include "log.h"
#include "const.h"
#include "redis.h"
#include "./json/writer.h"
#include "./json/features.h"
#include "./json/forwards.h"
#include "./json/json.h"
#include <stdlib.h>
#include <stdio.h>
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

#define INTERVALS 60 
#define SECINTERVALS 3
extern class CRedis rds;
class OnlineUsersInfo
{
	public:
		std::string m_company_id;
		int m_curr_company_users;
		int m_max_company_users;
		int m_min_company_users;
		int m_vir_company_users;
};
void DetectionTimeOut(const boost::system::error_code& e ,boost::asio::deadline_timer* t,void* param);
int SetVirOnlineUsers(int company_num,int& vir_num);

#endif
