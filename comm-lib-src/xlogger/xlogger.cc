//
// Created by derek on 12/30/16.
//
#include <mutex>
#include<stdarg.h>
#include <glog/logging.h>
#include "xlogger.h"

using namespace snetwork_xservice_xflagger;

static std::mutex vlock;
static std::string projectname = "xlogger";

int XLogger::verbose = 0;
XLogger* XLogger::mobject = nullptr;

XLogger* XLogger::GetObject(const char* pro) {

    if(nullptr == XLogger::mobject) {
		std::lock_guard<std::mutex> lock(vlock);
		if (nullptr == XLogger::mobject) {
			XLogger::mobject = new XLogger(pro);
		}
    }

	return XLogger::mobject;
}

void XLogger::FreeObject(void){
    if(nullptr!=XLogger::mobject) {
        delete XLogger::mobject;
        XLogger::mobject = nullptr;
    }
}

XLogger::XLogger(const char* pro) {
    projectname = pro;
    google::InitGoogleLogging(pro);
}

XLogger::~XLogger(){
	std::string().swap(projectname);
    google::ShutdownGoogleLogging();
}

void XLogger::Config(const std::string& dir, int verbose){
    std::lock_guard<std::mutex> lock(vlock);
    FLAGS_log_dir = dir;
    XLogger::verbose = verbose;
}

XLogger& XLogger::Dir(const std::string& dir){
    FLAGS_log_dir =  dir;
    return *this;
}

XLogger& XLogger::Verbose(const int verbose) {
    std::lock_guard<std::mutex> lock(vlock);
    XLogger::verbose = verbose;
    return  *this;
}

XLogger& XLogger::Colour(const bool b) {
    std::lock_guard<std::mutex> lock(vlock);
    FLAGS_colorlogtostderr = b;
    return  *this;
}

XLogger& XLogger::StdErr(const bool b) {
    std::lock_guard<std::mutex> lock(vlock);
    FLAGS_logtostderr = b;
    return  *this;
}


XLogger& XLogger::BuffDelay(const int s){
    std::lock_guard<std::mutex> lock(vlock);
	FLAGS_logbufsecs=s;
	return *this;
}


void XLogger::INFO(const char* file, size_t line, const std::string& info) {
	google::LogMessage(file, line).stream()<<info<<std::endl;
}

void XLogger::INFO(const char* file, size_t line, const char* fmt, ...) {
	char* msg = nullptr;
	va_list(arg);
	va_start(arg, fmt);
	vasprintf(&msg, fmt, arg);
	va_end(arg);
	google::LogMessage(file, line).stream()<<msg<<std::endl;
	free(msg);
}

void XLogger::WARNING(const char* file, size_t line, const std::string& war) {
	google::LogMessage(file, line, google::GLOG_WARNING).stream()<<war<<std::endl;
}

void XLogger::WARNING(const char* file, size_t line, const char* fmt, ...) {
	char* msg = nullptr;
	va_list(arg);
	va_start(arg, fmt);
	vasprintf(&msg, fmt, arg);
	va_end(arg);
	google::LogMessage(file, line, google::GLOG_WARNING).stream()<<msg<<std::endl;
	free(msg);
	msg = nullptr;
}

void XLogger::ERROR(const char* file, size_t line, const std::string& erro) {
	google::LogMessage(file, line, google::GLOG_ERROR).stream()<<erro<<std::endl;
}

void XLogger::ERROR(const char* file, size_t line, const char* fmt, ...) {
	char* msg = nullptr;
	va_list(arg);
	va_start(arg, fmt);
	vasprintf(&msg, fmt, arg);
	va_end(arg);
	google::LogMessage(file, line, google::GLOG_ERROR).stream()<<msg<<std::endl;
	free(msg);
	msg = nullptr;
}

void XLogger::INFO(const char* file, size_t line, const int v, const std::string& info) {
	!(v > XLogger::verbose) ? (void) 0 : google::LogMessageVoidify() & google::LogMessage(file, line).stream() << info << std::endl;
}

void XLogger::INFO(const char* file, size_t line, const int v, const char* fmt, ...) {
	char* msg = nullptr;
	va_list(arg);
	va_start(arg, fmt);
	vasprintf(&msg, fmt, arg);
	va_end(arg);
	!(v > XLogger::verbose) ? (void) 0 : google::LogMessageVoidify() & google::LogMessage(file, line).stream() << msg << std::endl;
	free(msg);
	msg = nullptr;
}

void XLogger::WARNING(const char* file, size_t line, const int v, const std::string& war) {
	!(v > XLogger::verbose) ? (void) 0 : google::LogMessageVoidify() & google::LogMessage(file, line, google::GLOG_WARNING).stream() << war << std::endl;
}

void XLogger::WARNING(const char* file, size_t line, const int v, const char* fmt, ...) {
	char* msg = nullptr;
	va_list(arg);
	va_start(arg, fmt);
	vasprintf(&msg, fmt, arg);
	va_end(arg);
	!(v > XLogger::verbose) ? (void) 0 : google::LogMessageVoidify() & google::LogMessage(file, line, google::GLOG_WARNING).stream() << msg << std::endl;
	free(msg);
	msg = nullptr;
}

void XLogger::ERROR(const char* file, size_t line, const int v, const std::string& erro) {
    LOG_IF(ERROR, v > XLogger::verbose) << erro << std::endl;
	!(v > XLogger::verbose) ? (void) 0 : google::LogMessageVoidify() & google::LogMessage(file, line, google::GLOG_ERROR).stream() << erro << std::endl;
}

void XLogger::ERROR(const char* file, size_t line, const int v, const char* fmt, ...) {
	char* msg = nullptr;
	va_list(arg);
	va_start(arg, fmt);
	vasprintf(&msg, fmt, arg);
	va_end(arg);
	!(v > XLogger::verbose) ? (void) 0 : google::LogMessageVoidify() & google::LogMessage(file, line, google::GLOG_ERROR).stream() << msg << std::endl;
	free(msg);
	msg = nullptr;
}

