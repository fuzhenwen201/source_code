//
// Created by derek on 12/30/16.
//

#ifndef XLOGGER_XLOGGER_H
#define XLOGGER_XLOGGER_H
#ifndef DISALLOW
#define DISALLOW
#define DISALLOW_COPY_AND_ASSIGN(TypeName)  \
    TypeName(const TypeName&);              \
    void operator=(const TypeName&)
#endif

#include <iostream>

#define XLOGGER(ProjectName) \
    (*(snetwork_xservice_xflagger::XLogger::GetObject(ProjectName)))

#define FREEXLOGGER \
    snetwork_xservice_xflagger::XLogger::FreeObject()

#define XLOG \
    snetwork_xservice_xflagger::XLogger

#define XINFO(msg,...) snetwork_xservice_xflagger::XLogger::INFO(__FILE__, __LINE__, msg, ##__VA_ARGS__)
#define XWARNING(msg,...) snetwork_xservice_xflagger::XLogger::WARNING(__FILE__, __LINE__, msg, ##__VA_ARGS__)
#define XERROR(msg,...) snetwork_xservice_xflagger::XLogger::ERROR(__FILE__, __LINE__, msg, ##__VA_ARGS__)

#define XVINFO(v, msg,...) snetwork_xservice_xflagger::XLogger::INFO(__FILE__, __LINE__, v, msg, ##__VA_ARGS__)
#define XVWARNING(v, msg,...) snetwork_xservice_xflagger::XLogger::WARNING(__FILE__, __LINE__, v, msg, ##__VA_ARGS__)
#define XVERROR(v, msg,...) snetwork_xservice_xflagger::XLogger::ERROR(__FILE__, __LINE__, v, msg, ##__VA_ARGS__)

namespace snetwork_xservice_xflagger {
    class XLogger {
     public:

        static XLogger* GetObject(const char* pro);
        static void FreeObject(void);
        virtual void Config(const std::string& dir, int verbose);
        virtual XLogger& Dir(const std::string& dir);
        virtual XLogger& Verbose(const int verbose);
        virtual XLogger& Colour(const bool b);
        virtual XLogger& StdErr(const bool b);
        virtual XLogger& BuffDelay(const int s);

        static void INFO(const char* file, size_t line, const std::string& info);
        static void INFO(const char* file, size_t line, const char* fmt, ...);

        static void WARNING(const char* file, size_t line, const std::string& war);
        static void WARNING(const char* file, size_t line, const char* fmt, ...);

        static void ERROR(const char* file, size_t line, const std::string& erro);
        static void ERROR(const char* file, size_t line, const char* fmt, ...);

        static void INFO(const char* file, size_t line, const int v, const std::string& info);
        static void INFO(const char* file, size_t line, const int v, const char* fmt, ...);

        static void WARNING(const char* file, size_t line, const int v, const std::string& info);
        static void WARNING(const char* file, size_t line, const int v, const char* fmt, ...);

        static void ERROR(const char* file, size_t line, const int v, const std::string& info);
        static void ERROR(const char* file, size_t line, const int v, const char* fmt, ...);

     private:
        DISALLOW_COPY_AND_ASSIGN(XLogger);
        XLogger();
        XLogger(const char* pro);
        virtual ~XLogger();

	 private:
        static XLogger* mobject;
        static int verbose;
    };
}
#endif //XLOGGER_XLOGGER_H
