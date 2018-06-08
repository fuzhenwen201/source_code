/*************************************************************************
  > File Name: http_request.cc
  > Author: xyz
  > Mail: xiao13149920@foxmail.com 
  > Created Time: Fri 03 Feb 2017 01:16:43 PM CST
 ************************************************************************/
#include<unistd.h>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include<rapidjson/rapidjson.h>
#include<rapidjson/stringbuffer.h>

#include"http_request.h"
#include"tnode_adapter.h"


namespace snetwork_xservice_solutiongateway {
/**** HttpRequest begin ****/
http_parser_settings HttpRequest::m_settings;
HttpRequest::HttpRequest() {
	m_settings.on_message_begin = OnMessageBegin;
	m_settings.on_headers_complete = OnHeadersComplete;
	m_settings.on_message_complete = OnMessageComplete;
	m_settings.on_url = OnUrl;
	m_settings.on_status = OnStatus;
	m_settings.on_header_field = OnHeaderField;
	m_settings.on_header_value = OnHeaderValue;
	m_settings.on_body = OnBody;
}

void HttpRequest::FreeWriteReq(uv_write_t* req) {
	write_req_t* wr = (write_req_t*) req;
	free(wr->buf.base);
	wr->buf.base = nullptr;
	//free(wr);
}

void HttpRequest::WriteCb(uv_write_t* req, int status) {
	if (status) {
		XERROR("Write error %s\n", uv_strerror(status));
	}

	FreeWriteReq(req);
}

/* callback of begin of parser*/
int HttpRequest::OnMessageBegin(http_parser* parser) {
#if defined(SIGNALENGINE)
	http_request_t* http_request = (http_request_t*)parser->data;
	http_request->header_lines = 0;
#endif

	return 0;
}

int HttpRequest::OnMessageComplete(http_parser *parser) {
	http_request_t* http_request = (http_request_t*)parser->data;

#if 0
	/* send response */
	write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));
	char* response = NULL;
	asprintf(&response, "%s", RESPONSE);
	req->buf = uv_buf_init(response, strlen(response));
	uv_write((uv_write_t*)&http_request->req, (uv_stream_t*)&http_request->stream, &req->buf, 1, nullptr);
	sleep(1);
	uv_write((uv_write_t*)&http_request->req, (uv_stream_t*)&http_request->stream, &req->buf, 1, nullptr);
	sleep(1);
	uv_write((uv_write_t*)&http_request->req, (uv_stream_t*)&http_request->stream, &req->buf, 1, nullptr);
	sleep(1);
	uv_write((uv_write_t*)&http_request->req, (uv_stream_t*)&http_request->stream, &req->buf, 1, WriteCb);
#endif
	return 0;
}

int HttpRequest::OnHeadersComplete(http_parser* parser) {
#if defined(SIGNALENGINE)
	http_request_t* http_request = (http_request_t*)parser->data;
	const char* method = http_method_str(http_method(parser->method));

	http_request->method = (char*)malloc(sizeof(method));
	strncpy(http_request->method, method, strlen(method));
	http_request->method [strlen(method)] = 0;
#endif
#if 0
	/* send response */
	client_t* client = (client_t*)parser->data;
	write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));
	char* response = NULL;
	asprintf(&response, "%s", RESPONSE);
	req->buf = uv_buf_init(response, strlen(response));
	uv_write((uv_write_t*)req, (uv_stream_t*)&client->handle, &req->buf, 1, WriteCb);
#endif

	return 0;
}

int HttpRequest::OnUrl(http_parser* parser, const char* buf, size_t len) {
#if defined(SIGNALENGINE)
	http_request_t* http_request = (http_request_t*)parser->data;

	http_request->url = (char*)malloc(len+1);
	if (http_request->url == nullptr) {
		XERROR("malloc failure");

		return -1;
	}

	strncpy((char*) http_request->url, buf, len);
	http_request->url[len] = 0;
#endif

	return 0;
}

int HttpRequest::OnStatus(http_parser* parser, const char* buf, size_t len) {

	return 0;
}

int HttpRequest::OnHeaderField(http_parser* parser, const char* buf, size_t len) {
#if defined(SIGNALENGINE)
	http_request_t* http_request = (http_request_t*)parser->data;
	http_header_t* header = &http_request->headers[http_request->header_lines];

	header->field = (char*)malloc(len+1);
	if (header->field == nullptr) {
		XERROR("malloc failure");

		return -1;
	}

	memset((char*)header->field, 0, len);
	strncpy((char*) header->field, buf, len);
	//header->field[len] = 0;
	header->field_length = len;
#endif

	return 0;
}

int HttpRequest::OnHeaderValue(http_parser* parser, const char* buf, size_t len) {
#if defined(SIGNALENGINE)
	http_request_t* http_request = (http_request_t*)parser->data;
	http_header_t* header = &http_request->headers[http_request->header_lines];

	header->value_length = len;
	header->value = (char*)malloc(len+1);
	if (header->value == nullptr) {
		XERROR("malloc failure");

		return -1;
	}

	memset((char*)header->value, 0, len+1);
	strncpy((char*) header->value, buf, len);
	//header->value[len] = 0;
	//header->value[len] = 0;
	++http_request->header_lines;
#endif

	return 0;
}

int HttpRequest::OnBody(http_parser* parser, const char* buf, size_t len) {
#if defined(SIGNALENGINE)
	http_request_t* http_request = (http_request_t*)parser->data;

	http_request->body = (char*)malloc(len+1);
	if (http_request->body == nullptr) {
		XERROR("malloc failure");

		return -1;
	}
	strncpy(http_request->body, buf, len);
	http_request->body[len] = 0;
#endif

#if 0
	rapidjson:: Document d;
	d.Parse(buf, len);

	if (d.HasParseError() || d.IsObject()) {
		XERROR("parser json error\n");

		return -1;
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	const char* output = buffer.GetString();
#if defined(DEBUG)
	XINFO("d.GetString: %s\n", output);
#endif
	rapidjson::Value::ConstMemberIterator it = d.FindMember("tradeType");
	if (it != d.MemberEnd()) {
#if defined(DEBUG)
		XINFO("tradeType: %s\n", it->value.GetString());
#endif
	}

	it = d.FindMember("name");
	if (it != d.MemberEnd()) {
#if defined(DEBUG)
		XINFO("name: %s\n", it->value.GetString());
#endif
	}

	snetwork_xservice_tnode::TNodeChannel* channel = TNodeAministrator::GetInstance()->GetChannel();
	char* msg = channel->AllocDataBuff(100);
	strcpy(msg, "hello world from solution gateway!");
	std::string group = TNodeAministrator::GetInstance()->GetSendOrderGroupName();
	std::string event = TNodeAministrator::GetInstance()->GetSendOrderGroupEvent();

#if 0
	channel->PublishGroup(TNodeAministrator::GetInstance()->GetSendOrderGroupName(), 
			TNodeAministrator::GetInstance()->GetSendOrderGroupEvent(), 
			msg, 
			(snetwork_xservice_tnode::uint32)(strlen(msg) + 1), 
			(snetwork_xservice_tnode::uint16)0);
#endif
	channel->PublishGroup(group, event, 
			msg, 
			(snetwork_xservice_tnode::uint32)(strlen(msg) + 1), 
			(snetwork_xservice_tnode::uint16)0);
#endif

	return 0;
}

bool HttpRequest::SendOrder() {
	return true;
}
/**** HttpRequest end ****/
}

