/*************************************************************************
  > File Name: connection.h
  > Author: xyz
  > Mail: xiao13149920@foxmail.com 
  > Created Time: Fri 03 Feb 2017 12:00:28 PM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_CONNECTION_H_
#define SOLUTIONGATEWAY_CONNECTION_H_

#include"define.h"
#include"order_list.h"
#include"http_parser.h"
#include"wrap_aes.h"
#include<mutex>
#include<map>

namespace snetwork_xservice_solutiongateway {
constexpr unsigned char TAG = 0x8F;
class HttpRequest;
class UVTcpServer {
	public:
		UVTcpServer();
		~UVTcpServer();

		UVTcpServer(const UVTcpServer& r) = delete;
		UVTcpServer& operator=(const UVTcpServer& r) = delete;
		UVTcpServer(UVTcpServer&& r) = delete;
		UVTcpServer& operator=(UVTcpServer&& r) = delete;

	public:
		int Run(void);
		const uv_loop_t* GetLoop(void) const {
			return m_loop;
		}
		uv_tcp_t GetTcpSever(void) const {
			return m_server;
		}
		struct sockaddr_in GetSockaddr(void) const {
			return m_addr;
		}

		static const char* GetSockIP(void) {
			return m_strAddr;
		}

		static unsigned short GetPort(void) {
			return m_port;
		}

		static const char* GetModuleIDStr(void) {
			return m_strSGID;
		}

		static int GetModuleID(void) {
			return m_iSGID;
		}

		static void OnClose(uv_handle_s* handle);

	protected:
#if defined(SIGNALENGINE)
		static void OnNewConnection(uv_stream_t* server, int status);
		static void EchoRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
		static void ParserJson(http_request_t* http_request); 
#endif
		static void OnNewConnectionTcp(uv_stream_t* server, int status);
		static void EchoReadTcp(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
		//static void OnClose(uv_handle_s* handle);
		int Init(void);
		static void AllocBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
		static void ParserJsonTcp(http_request_t* http_request, const uv_buf_t* buf, ssize_t nread);
	public:
		static void WriteCB(uv_write_t *req, int status);
		static void WriteReqCB(uv_async_t* async);

	public:
		/* tag: 0x11 */
		static void SendOrder(http_request_t* http_request, std::shared_ptr<char> body);
		/* tag: 0x21 */
		static void Login(http_request_t* http_request, std::shared_ptr<char> body);
		/* tag: 0x23 */
		static void ReLogin(http_request_t* http_request, std::shared_ptr<char> body);
		/* tag: 0x31 */
		static void Logout(http_request_t* http_request, std::shared_ptr<char> body);
		/* tag: 0x61 */
		static void ChangePWD(http_request_t* http_request, std::shared_ptr<char> body);

		struct UV_BUF_T {
			char*base;
			size_t len;
			size_t cap;
		};

	private:
		static char m_strSGID[10];
		static unsigned short m_port;
		static int m_iSGID;
		static char m_dbName[20];
		static char m_strAddr[INET_ADDRSTRLEN];
		static uv_loop_t*	m_loop;
		static uv_tcp_t	m_server;
		struct sockaddr_in	m_addr;
		static HttpRequest* m_settings;
		static std::once_flag m_once;
		static uv_buf_t m_buf;
		static std::map<void*, struct UV_BUF_T> m_map;
		const static int m_msgLen = 1024;

		static CAES m_aes;
};

}

#endif

