/*************************************************************************
	> File Name: mysqlop.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Sun 15 Jan 2017 08:15:31 PM PST
 ************************************************************************/

#ifndef DB_MYSQLOP_H_
#define DB_MYSQLOP_H_

#include<memory>	// for share_ptr
#include<exception>	// for exception
#include<mutex>		// for std::mutex
#include<string>
#include<mysql/mysql.h>
#include<forward_list>
#include<vector>

#define MAX_RECONNECTION	1025

namespace snetwork_xservice_db {
	struct ConnectionUnavailable : public std::exception {
		const char* what() const throw() {
			return "Unable to allocate connection";
		}
	};

	class MySql {
		public:
			MySql(const char* host, const char* user, const char* pwd, unsigned short port);
			~MySql();

		private:
			MySql(const MySql& r) = delete;
			MySql(MySql&& r) = delete;
			MySql& operator=(const MySql& r) = delete;
			MySql& operator=(MySql&& r) = delete;

		public:
			const std::string& GetHost(void) const {
				return m_host;
			}
			const std::string& GetUser(void) const {
				return m_user;
			}
			const std::string& GetPwd(void) const {
				return m_pwd;
			}
			unsigned short GetPort(void) const {
				return m_port;
			}
			unsigned char GetIndex(void) const {
				return m_index;
			}
			void SetIndex(unsigned char index) {
				m_index = index;
			}

			MYSQL* GetMySql(void) {
				return m_pMySql;
			}
			void SetMySQL(MYSQL* mysql) {
				m_pMySql = mysql;
			}

			time_t GetActiveTime(void) const {
				return m_activeTime;
			}
			void SetActiveTime(time_t activeTime) {
				m_activeTime = activeTime;
			}

			size_t GetWaitTimeOut(void) const {
				return m_waitTimeOut;
			}
			void SetWaitTimeOut(size_t waitTimeOut) {
				m_waitTimeOut = waitTimeOut;
			}

		private:
			std::string m_host;
			std::string m_user;
			std::string m_pwd;
			unsigned short m_port;
			unsigned char m_index;
			MYSQL* m_pMySql;		// connection
			time_t m_activeTime;	
			size_t m_waitTimeOut;
			size_t m_reConnectCnt; /* for reconnection*/
	};

	class ConnectionPool {
		public:
			static ConnectionPool* GetInstance();
			~ConnectionPool();
			MySql* GetConnect(bool& isConnect);
			void ReConnect(MySql* mysql, bool& isConnect);
			void ReleaseConnect(MySql* mysql);
			

		private:
				ConnectionPool(unsigned char max = 20);

				void Init(const char* host="127.0.0.1", const char* user="root", const char* pwd="root", short port=3306);

				ConnectionPool(const ConnectionPool& r)=delete;
				ConnectionPool& operator=(const ConnectionPool& r)=delete;
				ConnectionPool(ConnectionPool&& r)=delete;
				ConnectionPool& operator=(ConnectionPool&& r)=delete;

		protected:
				/* for init pool*/
				void FirstInit(const char* host, const char* user, const char* pwd, short port);
				void SecondInit(const char* host, const char* user, const char* pwd, short port);

		private:
				const size_t m_maxConnection;  
				size_t m_currConnection;  
				size_t m_waitTimeOut;	// database default wait_timeout
				std::forward_list<MySql*> m_list;
				static std::mutex m_mutex;
				static ConnectionPool* m_pInstance;
	};
	
	class MySqlRecordSet;
	class MySqlDB;
	/* database operator: select/insert/delete etc*/
	class Field {
		friend class MySqlDB;
		friend class MySqlRecordSet;

		public:
			Field(const char* name=NULL, size_t index=0);
			Field(const std::string& name, size_t index=0);
			Field(std::string&& name, size_t index=0);
			~Field();
			Field(const Field& r);
			Field(Field&& r);
			Field& operator=(const Field& r);
			Field& operator=(Field&& r);
		public:
		private:
			std::string m_name;
			size_t m_index;
	};

	class MySqlRecordSet {
		public:
			MySqlRecordSet(size_t rows=0, size_t cols=0);
			~MySqlRecordSet();

			MySqlRecordSet(const MySqlRecordSet& r);
			MySqlRecordSet(MySqlRecordSet&& r);
			MySqlRecordSet& operator=(const MySqlRecordSet& r);
			MySqlRecordSet& operator=(MySqlRecordSet&& r);

		public:
			void SetRows(size_t rows) {
				m_recordSets.reserve(rows);
				m_rows = rows;
			}
			void SetCols(size_t cols) {
				m_fields.reserve(cols);
				m_cols = cols;
			}
			void SetSize(size_t rows, size_t cols) {
				m_recordSets.reserve(rows);
				m_rows = rows;
				m_fields.reserve(cols);
				m_cols = cols;
			}

			const std::string& GetFieldByName(size_t row, const std::string& colName) const;
			const std::string& GetFieldByName(size_t row, const char* colName) const;

			const std::string& GetFieldByID(size_t row, size_t col) const;

			size_t GetRows(void) const {
				return m_rows;
			}
			size_t GetCols(void) const {
				return m_cols;
			}

			ssize_t GetFieldIndex(const char* name) const;
			ssize_t GetFieldIndex(const std::string& name) const;
			const std::string& GetFieldName(size_t col) const {
				if (col >= m_cols) {
					return m_nullString;
				}

				return m_fields[col].m_name;
			}

			std::vector<Field>& GetFields(void) {
				return m_fields;
			}

			std::vector<std::vector<std::string>>& GetRecordSets(void) {
				return m_recordSets;
			}

		private:
			size_t m_rows;
			size_t m_cols;
			std::vector<Field> m_fields;
			std::vector<std::vector<std::string>> m_recordSets;
			std::string m_nullString;
	};

	class MySqlDB {
		public:
			MySqlDB();
			~MySqlDB();

			MySqlDB(const MySqlDB& r) = delete;
			MySqlDB& operator=(const MySqlDB& r) = delete;
			MySqlDB(MySqlDB&& r) = delete;
			MySqlDB& operator=(MySqlDB&& r) = delete;

			const MySql* Open(int connectID=0); /* reserver for multhostle databse(different host/user/port etc)*/
			int Close(void);
			/* 0: success
			 * other: fail
			 * */
			int SelectDB(const char* dbName);
			/*-1: faile
			 * other: affected rows*/
			ssize_t ExecSql(const char* sql);
			MySqlRecordSet QuerySql(const char* sql);
			ssize_t GetInsertID(void);
			void StartTransaction(void);
			void Commint(void);
			void RollBack(void);

			bool IsError(void) const {
				return m_bError;
			}
			void SetError(bool error) {
				m_bError = error;
			}
			const char* GetMySqlErrMsg(void);
			bool IsConnect(void) const {
				return m_bConnect;
			}

		private:
			bool m_bConnect;
			bool m_bError;
			/* connect id: revserve for mutiple host/user/port etc */
			int m_connectID;
			MySql* m_pMySql;
	};
}


#endif // DB_MYSQLOP_H_

