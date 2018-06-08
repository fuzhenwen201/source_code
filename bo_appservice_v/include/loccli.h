#ifndef _WORK_H_
#define _WORK_H_

#include "log.h"
#include "const.h"
#include "redis.h"
#include "kerner.h"
#include "msgopr.h"
#include "memopr.h"
#include "common.h"
#include "tyinhttp.h"
#include "./json/writer.h"
#include "./json/features.h"
#include "./json/forwards.h"
#include "./json/json.h"
#include "./curl/curl.h"

#include <map>
#include <list>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
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


#include <deque>  
#include <boost/thread/thread.hpp>  



#include <boost/asio/deadline_timer.hpp>  
#include <boost/asio/io_service.hpp>  
#include <boost/asio/ip/tcp.hpp>  
#include <boost/asio/read_until.hpp>  
#include <boost/asio/streambuf.hpp>  
#include <boost/asio/write.hpp>  
#include <boost/bind.hpp>  
#include <iostream>  
  
using boost::asio::deadline_timer;  
using boost::asio::ip::tcp;  


void Write(void* param);

class client
{
	public:
		// 在构造函数的初始化列表中 初始化所有变量
		client(boost::asio::io_service& io_service);
		void start(tcp::resolver::iterator endpoint_iter);
		void writedata(char* data);
	private:
		void stop();
		void start_connect(tcp::resolver::iterator endpoint_iter);
		void handle_connect(const boost::system::error_code& ec,tcp::resolver::iterator endpoint_iter);
		void start_read();
		void handle_read_header(const boost::system::error_code& error);
		void handle_read_body(const boost::system::error_code& error);
		void start_write();
		void handle_write(const boost::system::error_code& ec);
		void check_deadline();
	private:
		char reqlen[LOC_HEADER_LEN+1];
		char data_[LOC_BODY_LEN+1];
		bool stopped_;                              // 标识程序是否停止运行
		tcp::socket socket_;                        //
		boost::asio::streambuf input_buffer_;       // recv buf
		deadline_timer deadline_;                   // 连接/recv 超时 定时器
		deadline_timer heartbeat_timer_;            // 心跳超时 计时器
};


#if 0
using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::system::error_code;

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#define BOOL             int
#define INVALID_SOCKET    -1
#define SOCKET_ERROR     -1
#define TRUE             1
#define FALSE             0


static time_t oldtime = 0, nowtime = 0;

boost::mutex cout_mtx;
int packet_size = 0;
enum {MAX_PACKET_LEN = 4096};

class io_service_pool : public boost::noncopyable
{
	public:


		void join();
		void stop();
		void start();
		explicit io_service_pool(std::size_t pool_size);
		~io_service_pool();
		boost::asio::io_service& get_io_service();

	private:
		typedef boost::shared_ptr<boost::asio::io_service> io_service_sptr;
		typedef boost::shared_ptr<boost::asio::io_service::work> work_sptr;
		typedef boost::shared_ptr<boost::thread> thread_sptr;
		boost::mutex mtx;
		std::vector<io_service_sptr> io_services_;
		std::vector<work_sptr> work_;
		std::vector<thread_sptr> threads_; 
		std::size_t next_io_service_;
};


class session
{
	public:
		virtual ~session();
		inline void start();
		tcp::socket& socket();
		session(boost::asio::io_service& io_service);
		void handle_write(const boost::system::error_code& error);
		void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

	private:
		tcp::socket socket_;
		char data_[MAX_PACKET_LEN];
		int recv_times;
};

class server
{
	public:
		void run();
		server(short port, int thread_cnt);
		void handle_accept(session* new_session, const boost::system::error_code& error);

	private:
		io_service_pool io_service_pool_;
		tcp::acceptor acceptor_;
};
#endif
#endif