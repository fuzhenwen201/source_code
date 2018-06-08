/*************************************************************************
	> File Name: SEconfig.h
	> Author: kevin
	> Mail: kevin@1stellar.com
	> Created Time: Wednesday, April 19, 2017 PM02:49:50 HKT
 ************************************************************************/
#ifndef SECONFIG_H_
#define SECONFIG_H_

#include "xconfig.h"
#include <ostream>

namespace snetwork_xservice_xflagger {

	class SEConfig : public XConfig {
		public:

			SEConfig();
			virtual ~SEConfig();

			SEConfig(const SEConfig & s) = delete;
			SEConfig& operator=(const SEConfig & s) = delete;
			virtual void Init(void);

		public:

			/*log*/
			void LoggerDir(const std::string& loggerDir) {
				this->loggerDir = loggerDir;
			}

			const std::string& LoggerDir(void) const {
				return loggerDir;
			}

			void LoggerFile(const std::string& loggerFile) {
				this->loggerFile = loggerFile;
			}

			const std::string& LoggerFile(void) const {
				return loggerFile;
			}

			void AccessToken(const std::string& accessToken) {
				this->accessToken = accessToken;
			}

			const std::string& AccessToken(void) const {
				return accessToken;
			}

			void Domain(const std::string& domain) {
				this->domain = domain;
			}

			const std::string& Domain(void) const {
				return domain;
			}

			/*rabbitmq*/
			void VHost(const std::string& vhost) {
				mqvhost = vhost;
			}

			const std::string& VHost(void) const {
				return mqvhost;
			}

			void Exchange(const std::string& exchange) {
				mqExchange = exchange;
			}
			
			const std::string& Exchange(void) const {
				return mqExchange;
			}

			void ExchangeGroup(const std::string& exc) {
				mqExchangeGroup = exc;
			}
			
			const std::string& ExchangeGroup(void) const {
				return mqExchangeGroup;
			}

			void Address(const std::string& addr) {
				mqAddress = addr;
			}

			const std::string& Address(void) const {
				return mqAddress;
			}

			void Port(const std::string& port) {
				mqPort = port;
			}

			const std::string& Port(void) const {
				return mqPort;
			}

			void User(const std::string& user) {
				mqUser = user;
			}

			const std::string& User(void) const {
				return mqUser;
			}

			void Password(const std::string& password) {
				mqPassword = password;
			}

			const std::string& Password(void) const {
				return mqPassword;
			}

			friend std::ostream& operator<<(std::ostream& os, const SEConfig& s);
		private:
			std::string loggerDir;
			std::string loggerFile;
			std::string accessToken;
			std::string domain;
			std::string mqvhost;
			std::string mqExchange;
			std::string mqExchangeGroup;
			std::string mqAddress;
			std::string mqPort;
			std::string mqUser;
			std::string mqPassword;
	};

	class SEFlagger : public XFlagger {
		public:

			static SEFlagger* GetInstance(void);
		    static void FreeInstance(void);
			virtual void FlagXConfig(XConfig* cfg);
    		virtual XConfig* GetConfig(void) const {	
				return m_config;
			}

		private:

			SEFlagger();
			virtual ~SEFlagger();

			SEFlagger(const SEFlagger& s) = delete;
			SEFlagger& operator=(const SEFlagger& s) = delete;

		private:

			XConfig* m_config;
			static SEFlagger* m_seflagger;
	};

}
#endif

