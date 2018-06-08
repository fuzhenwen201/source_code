#ifndef _SESSION_H_
#define _SESSION_H_
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

using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::system::error_code;
class session
{
	public:
		virtual ~session();
		inline void start();
		tcp::socket& socket();
		session(boost::asio::io_service& io_service);
		void handle_write(const boost::system::error_code& error);
		void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
		void handle_read_header(const boost::system::error_code& error);
		void handle_read_body(const boost::system::error_code& error);
		void write(char* data);
		//void handle_write_heart(const boost::system::error_code& ec);
		//void start_write();


	private:
		char reqlen[LOC_HEADER_LEN+1];
		tcp::socket socket_;
		char data_[LOC_BODY_LEN];
		int recv_times;
		//deadline_timer heartbeat_timer_;
};
#endif