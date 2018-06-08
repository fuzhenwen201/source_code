#ifndef _MEMOPRBOOST_H_
#define _MEMOPRBOOST_H_

#include <boost/interprocess/managed_shared_memory.hpp>  
#include <boost/interprocess/allocators/allocator.hpp>  
#include <boost/interprocess/containers/map.hpp>  
#include <boost/interprocess/containers/string.hpp> 

#include <unistd.h>
//#include <iostream>  

//using namespace boost;
using namespace boost::interprocess; 
typedef  boost::interprocess::managed_shared_memory 									manage_sharememory;
typedef  boost::interprocess::managed_shared_memory::segment_manager                       segment_manager_t;  
typedef  boost::interprocess::allocator<void, segment_manager_t>                           void_allocator;  
typedef  boost::interprocess::allocator<int, segment_manager_t>                            int_allocator;  
typedef  boost::interprocess::allocator<char, segment_manager_t>                           char_allocator;  
typedef  boost::interprocess::basic_string<char, std::char_traits<char>, char_allocator>   char_string;


class ShareMemBuf
{
public:
	char_string m_crmtoken;
	int m_crmtime;
	int m_lasttime;
	char_string m_crmpwd;
	//char_string m_companyid;
	ShareMemBuf(const char* crmtoken ,const char* crmpwd,const void_allocator &void_alloc,int app_time,int crm_time);
	~ShareMemBuf();

} ;


typedef std::pair<const char_string, ShareMemBuf>                      map_value_type;  
typedef boost::interprocess::allocator<map_value_type, segment_manager_t>                    map_value_type_allocator;  
typedef boost::interprocess::map<char_string, ShareMemBuf, std::less<char_string>, map_value_type_allocator>     buf_map_type;  

class CIpcMemBoost
{
public:
	//static int Delete(int shmid);

public:
	CIpcMemBoost();
	~CIpcMemBoost();

public:
	int Create(key_t shmkey,int type);
	buf_map_type* GetBuf();

	int InsertData(char* apptoken,char* crmtoken,char* crmpwd,int time,int crm_time);
	void DeleteData(char*apptoken,char* crmtoken);
	int UpdateData(char* apptoken,char* crmtoken,char* crmpwd,int time,int crm_time);
	int FindCrmtoken(char* apptoken, char* crmtoken);
	int FindCrmToken(char* apptoken, char*crmtoken,char* crmpwd,int& time,int& crm_time);
	//int GetShmid();
	//int Detach();
protected:
	int Delete();

private:
	int shmId;
	//pIpcMemBuf shMem;
	buf_map_type *m_buf_map; 
	managed_shared_memory* m_shareId;
	void_allocator* m_alloc_inst;
};

#endif