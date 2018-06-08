#include "../include/dbsopr.h"

CDbOpr::CDbOpr()
{
	//初始化连接数据库变量
	pConn = mysql_init(NULL);
	pConn != NULL ?
		LOG_INFO("mysql_init sucess"):
		LOG_ERR("mysq_init err[%d]",mysql_errno(pConn));
}

CDbOpr::~CDbOpr()
{
	//关闭初始化连接数据库变量
	if(pConn != NULL)
	{
		mysql_close(pConn);
	}
}

bool CDbOpr::InitDb(string host, string user, string pwd, string dbname)
{
	//运用mysql_real_connect函数实现数据库的连接
	M_XRETURN(!mysql_real_connect(pConn , host.c_str() , user.c_str() ,\
				pwd.c_str() , dbname.c_str() , 0 , NULL , 0), false, \
			LOG_ERR("mysq_real_connect err[%d]",mysql_errno(pConn)));
	LOG_INFO("mysq_real_connect succ");
	return true;
}

void CDbOpr::ClsMap()
{
	map<string ,string>::iterator itMap;;
	for (map<string, string>::iterator i=redMap.begin(); i!=redMap.end(); )
	{
		redMap.erase(itMap); 
		i++;
	}
}

bool CDbOpr::ExecuteSqlS(string strTblN, void * vd)
{
	MYSQL_FIELD *field;
	string sql;
	sql = "select * from " + strTblN;
	int offset;
	void * pst;

	//char szFldName[DB_COL_MAX][DB_FLD_MAX];
	ClsMap();
	char szFldName[1500][200]={0};
	M_XRETURN(strTblN == "", false, LOG_ERR("sql is NULL"));

	if(strTblN == "emp")
	{
		pst = (pTbl_emp)vd;;
	}

	// 查询编码设定
	M_XRETURN(mysql_query(pConn, "set names utf8"), false,
			LOG_ERR("mysql_query err[%d],[%s]",mysql_errno(pConn), mysql_error(pConn)));

	M_XRETURN(mysql_query(pConn,  sql.c_str()), false,
			LOG_ERR("Error making query: %s\n" , mysql_error(pConn)));

	//初始化逐行的结果集检索
	pRes = mysql_use_result(pConn);

	int i = 0;
	//提取字段
	for(i=0; field = mysql_fetch_field(pRes);i++)
	{
		memcpy(szFldName[i], field->name, strlen(field->name));
		LOG_INFO("field name[%s]i[%d]", szFldName[i],i);
	}

	if(pRes)
	{
		//取查询的结果集
		while(row=mysql_fetch_row(pRes))
		{
			//函数返回结果集中字段的数
			for(int r = 0, i = 0; r  < mysql_num_fields(pRes) ; r++,i++)
			{
				//	offset = OFFSET_OF(Tbl_Emp, szFldName[i]);
				//	strcpy(pst+offset, row[r]);
				redMap.insert(make_pair(szFldName[i],row[r]));
				LOG_INFO("%s[%s]i[%d]",szFldName[i],row[r],i);
			}
		}
	}
	LOG_INFO("sql[%s] exec sucess",sql.c_str());
	//释放结果集使用的内存
	mysql_free_result(pRes);

	return true;
}

//修改,删除,插入数据
bool CDbOpr::ExecuteSqlM(string sql)
{
	M_XRETURN(sql == "", false, LOG_ERR("sql is NULL"));

	M_XRETURN(mysql_query(pConn,  sql.c_str()), false,
			LOG_ERR("Del Err: %s\n" , mysql_error(pConn)));

	LOG_INFO("sql[%s] exec sucess",sql.c_str());
	return true;
}

//执行不返回结果集的存储过程
bool CDbOpr::ExecProcudere(string sql)
{
	int nRet = 0;
	MYSQL_ROW results,record;
	M_XRETURN(sql == "", false, LOG_ERR("sql is NULL"));
	M_XRETURN(mysql_real_query(pConn, sql.c_str(), (unsigned int)sql.length()), false,
			LOG_ERR("Exec procude Err: %s\n" , mysql_error(pConn)));

	mysql_query(pConn, "SELECT @szRet ");

	pRes = mysql_store_result(pConn);
	/** 输出返回值 **/
	while((row = mysql_fetch_row(pRes)))
	{
		//函数返回结果集中字段的数
		for(int r = 0, i = 0; r  < mysql_num_fields(pRes) ; r++,i++)
		{
			LOG_INFO("Ret[%s]i[%d]",row[r],i);
			nRet = atoi(row[r]);
		}
	}
	mysql_free_result(pRes);

	M_XRETURN(nRet != 0, false,
			LOG_ERR("Exec procude faile: %d" , nRet));

	LOG_INFO("sql[%s] exec sucess",sql.c_str());
	return true;
}

//执行返回结果集的存储过程
bool CDbOpr::ExecProcudere(string sql, map<int, string> *retMap)
{
	int nRet = 0;
	int nDex = 1;
	char szGetV[128];
	char *sPos = NULL;
	char *ePos = NULL;

	memset(szGetV, 0x00, sizeof(szGetV));
	MYSQL_ROW results,record;


	M_XRETURN(sql == "", false, LOG_ERR("sql is NULL"));
	
	sPos = strstr((char*)sql.c_str(), "(");
	ePos = strstr((char*)sql.c_str(),")");

	M_XRETURN( sPos== NULL || ePos == NULL, false, LOG_ERR("sql fromat Err"));
	//M_XRETURN(mysql_real_query(pConn, sql.c_str(), (unsigned int)sql.length()), false,
	M_XRETURN(mysql_query(pConn, sql.c_str()), false,
			LOG_ERR("Exec procude Err: %s" , mysql_error(pConn)));

	memcpy(szGetV+7, sPos+1, ePos-sPos-1);
	memcpy(szGetV, "SELECT ", 7);
	LOG_INFO("GetV[%s] ", szGetV);

	//mysql_query(pConn, "SELECT @nRet,@name,@age ");
	mysql_query(pConn, szGetV);

	pRes = mysql_store_result(pConn);
	/** 输出返回值 **/
	while((row = mysql_fetch_row(pRes)))
	{
		
		//函数返回结果集中字段的数
		for(int r = 0, i = 0; r  < mysql_num_fields(pRes) ; r++,i++)
		{
			row[r] == NULL ? 
				retMap->insert(make_pair((i+1)*nDex,"null")) :
				retMap->insert(make_pair((i+1)*nDex,row[r])) ;
			LOG_INFO("Ret[%s]i[%d]r[%d]",row[r],i,r);
			if(nDex == 1 && i == 0) nRet = atoi(row[r]);
		}
		nDex++;
	}
	mysql_free_result(pRes);

	M_XRETURN(nRet != 0, false,
			LOG_ERR("Exec procude faile: %d" , nRet));

	LOG_INFO("sql[%s] exec sucess",sql.c_str());
	return true;
}

int CDbOpr::BeginTransaction()
{
	//mysql_query("BEGIN");
	M_XRETURN( mysql_query(pConn, "BEGIN" ) , false,
			LOG_ERR("BEGIN Err: %s" , mysql_error(pConn)));
}

int CDbOpr::Commit()
{
	M_XRETURN( mysql_query(pConn, "COMMIT" ) , false,
			LOG_ERR("COMMIT Err: %s" , mysql_error(pConn)));
}

int CDbOpr::Rollback()
{
	M_XRETURN( mysql_query(pConn, "ROLLBACK" ) , false,
			LOG_ERR("Rollback Err: %s" , mysql_error(pConn)));
}

void CDbOpr::PrtRed()
{
	map<string,string>::iterator   it=redMap.begin();   
	for(;it!=redMap.end();++it)
	{
		LOG_INFO("fileName[%s]Value[%s]",it->first.c_str(), it->second.c_str());
	}
}

bool CDbOpr::CreateTb(string table_str_sql)
{
	return true;
}
