#ifndef _DBSOPR_H_
#define _DBSOPR_H_

#include "log.h"
#include "const.h"

#include<map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "/usr/include/mysql/mysql.h"
using namespace std;

class CDbOpr
{
	public:
		CDbOpr();
		~CDbOpr();
		bool InitDb(string host, string user, string pwd, string dbname);
		//bool ExecuteSql(string sql);
		bool ExecuteSqlS(string strTblN, void * vd);
		bool ExecuteSqlM(string sql);
		bool ExecProcudere(string sql);
		bool ExecProcudere(string sql, map<int, string> *retMap);
		bool CreateTb(string table_str_sql);
		void ClsMap();
		void PrtRed();
		int BeginTransaction();
		int Commit();
		int Rollback();
	private:
			MYSQL *pConn;
			MYSQL_RES *pRes;
			MYSQL_ROW row;
			map<string ,string> redMap; 
};


typedef struct stTbl_Emp
{
	char id[20];
	char ename[50];
	char age[20];
	char job[50];
}Tbl_Emp, *pTbl_emp;

typedef struct TblNameMap
{
	string strTblName;
	string strStNmae;
}TblNameMap, *pTblNameMap;

#endif 
