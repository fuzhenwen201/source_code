/*************************************************************************
	> File Name: http_request.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Fri 03 Feb 2017 01:59:22 PM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_HTTP_REQUEST_H_
#define SOLUTIONGATEWAY_HTTP_REQUEST_H_

#include"define.h"
#include"order_list.h"

namespace snetwork_xservice_solutiongateway {
class HttpRequest {
	public:
		HttpRequest();
		~HttpRequest() = default;

		HttpRequest(const HttpRequest& r) = delete;
		HttpRequest(HttpRequest&& r) = delete;
		HttpRequest& operator=(const HttpRequest& r) = delete;
		HttpRequest& operator=(HttpRequest&& r) = delete;

	public:
		static void FreeWriteReq(uv_write_t* req);
		static void WriteCb(uv_write_t* req, int status);
		static int OnMessageBegin(http_parser* parser);
		static int OnMessageComplete(http_parser *parser);
		static int OnHeadersComplete(http_parser* parser);
		static int OnUrl(http_parser* parser, const char* buf, size_t len);
		static int OnStatus(http_parser* parser, const char* buf, size_t len);
		static int OnHeaderField(http_parser* parser, const char* buf, size_t len);
		static int OnHeaderValue(http_parser* parser, const char* buf, size_t len);
		static int OnBody(http_parser* parser, const char* buf, size_t len);
		http_parser_settings* GetSettings(void) const {
			return &m_settings;
		}
		
		bool SendOrder();

	private:
		static http_parser_settings m_settings;
};

};

#endif
