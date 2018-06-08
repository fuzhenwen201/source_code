#ifndef _LOCSERVER_H_
#define _LOCSERVER_H_

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
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <sys/time.h>
#include <sstream>
#include <map>
#include <list>

#include <hash_map>   
#include <boost/bind.hpp>   
#include <boost/asio.hpp>   
#include <boost/shared_ptr.hpp>   
#include <boost/make_shared.hpp>   
#include <boost/thread.hpp>   
#include <boost/thread/mutex.hpp>   
#include <boost/enable_shared_from_this.hpp>   
#if 0
#include <firebird/log/logger_log4.hpp>   
#include <firebird/detail/config.hpp>   
#include <firebird/socket_utils/message_archive.hpp>  
#endif


using boost::asio::ip::tcp;  

namespace firebird{  
	enum command{ heartbeat = 0, regist, normal};  

	const std::string tag = "KDS";  

	class FIREBIRD_DECL socket_session;  
	typedef boost::shared_ptr<socket_session> socket_session_ptr;  

	class FIREBIRD_DECL socket_session:  
		public boost::enable_shared_from_this<socket_session>,  
		private boost::noncopyable  
	{  
		public:  
			typedef boost::function<void(socket_session_ptr)> close_callback;  
			typedef boost::function<void(  
					const boost::system::error_code&,   
					socket_session_ptr, message&)> read_data_callback;  

			socket_session(boost::asio::io_service& io_service);  
			~socket_session(void);  

			DWORD id() { return m_id; }  
			WORD get_business_type(){ return m_business_type; }  
			void set_business_type(WORD type) { m_business_type = type; }  
			DWORD get_app_id(){ return m_app_id; }  
			void set_app_id(DWORD app_id) { m_app_id = app_id; }  
			std::string& get_remote_addr() { return m_name; }  
			void set_remote_addr(std::string& name) { m_name = name; }  
			tcp::socket& socket() { return m_socket; }  

			void installCloseCallBack(close_callback cb){ close_cb = cb; }  
			void installReadDataCallBack(read_data_callback cb) { read_data_cb = cb; }  

			void start();  
			void close();  
			void async_write(const std::string& sMsg);  
			void async_write(message& msg);  

			bool is_timeout();  
			void set_op_time(){std::time(&m_last_op_time);}  

		private:  
			static boost::detail::atomic_count m_last_id;  

			DWORD m_id;  
			WORD  m_business_type;  
			DWORD m_app_id;  
			std::string m_name;  
			boost::array<char, 7> sHeader;  
			std::string sBody;  

			tcp::socket m_socket;  
			boost::asio::io_service& m_io_service;  

			std::time_t m_last_op_time;  

			close_callback close_cb;  
			read_data_callback read_data_cb;  

			//发送消息   
			void handle_write(const boost::system::error_code& e,   
					std::size_t bytes_transferred, std::string* pmsg);  

			//读消息头   
			void handle_read_header(const boost::system::error_code& error);  
			//读消息体   
			void handle_read_body(const boost::system::error_code& error);  

			void handle_close();  
	};
}
using boost::asio::ip::tcp;  

namespace firebird{  
	enum command{ heartbeat = 0, regist, normal};  

	const std::string tag = "KDS";  

	class FIREBIRD_DECL socket_session;  
	typedef boost::shared_ptr<socket_session> socket_session_ptr;  

	class FIREBIRD_DECL socket_session:  
		public boost::enable_shared_from_this<socket_session>,  
		private boost::noncopyable  
	{  
		public:  
			typedef boost::function<void(socket_session_ptr)> close_callback;  
			typedef boost::function<void(  
					const boost::system::error_code&,   
					socket_session_ptr, message&)> read_data_callback;  

			socket_session(boost::asio::io_service& io_service);  
			~socket_session(void);  

			DWORD id() { return m_id; }  
			WORD get_business_type(){ return m_business_type; }  
			void set_business_type(WORD type) { m_business_type = type; }  
			DWORD get_app_id(){ return m_app_id; }  
			void set_app_id(DWORD app_id) { m_app_id = app_id; }  
			std::string& get_remote_addr() { return m_name; }  
			void set_remote_addr(std::string& name) { m_name = name; }  
			tcp::socket& socket() { return m_socket; }  

			void installCloseCallBack(close_callback cb){ close_cb = cb; }  
			void installReadDataCallBack(read_data_callback cb) { read_data_cb = cb; }  

			void start();  
			void close();  
			void async_write(const std::string& sMsg);  
			void async_write(message& msg);  

			bool is_timeout();  
			void set_op_time(){std::time(&m_last_op_time);}  

		private:  
			static boost::detail::atomic_count m_last_id;  

			DWORD m_id;  
			WORD  m_business_type;  
			DWORD m_app_id;  
			std::string m_name;  
			boost::array<char, 7> sHeader;  
			std::string sBody;  

			tcp::socket m_socket;  
			boost::asio::io_service& m_io_service;  

			std::time_t m_last_op_time;  

			close_callback close_cb;  
			read_data_callback read_data_cb;  

			//发送消息   
			void handle_write(const boost::system::error_code& e,   
					std::size_t bytes_transferred, std::string* pmsg);  

			//读消息头   
			void handle_read_header(const boost::system::error_code& error);  
			//读消息体   
			void handle_read_body(const boost::system::error_code& error);  

			void handle_close();  
	};  
}  

#endif
