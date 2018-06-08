#include "../../../include/memopr_boost.h"
#include "../../../include/log.h"
#include "../../../include/const.h"


ShareMemBuf::ShareMemBuf(const char* crmtoken,const char* crmpwd,const void_allocator &void_alloc,int app_time,int crm_time) :
	 		  m_crmtoken(crmtoken,void_alloc),m_crmpwd(crmpwd,void_alloc),m_lasttime(app_time),m_crmtime(crm_time)
{

}
ShareMemBuf::~ShareMemBuf()
{

}

CIpcMemBoost::CIpcMemBoost():m_shareId(NULL),m_alloc_inst(NULL),m_buf_map(NULL)
{

}

CIpcMemBoost::~CIpcMemBoost()
{
	Delete();
}
int CIpcMemBoost::Create(key_t shmkey,int type)
{
	char name[20] = { 0 };
	sprintf(name,"%d",shmkey);
	LOG_INFO("shmkey:[%s]",name);
	if(type == 0)
	{
		//shared_memory_object::remove(name); 
		m_shareId = new managed_shared_memory(open_or_create, name, 65536); 
		m_alloc_inst = new void_allocator(m_shareId->get_segment_manager()) ;
		m_buf_map = m_shareId->find<buf_map_type>("MyMap").first;
		if(NULL == m_buf_map)
			m_buf_map = m_shareId->construct<buf_map_type>("MyMap")(std::less<char_string>(), *m_alloc_inst);
		else 
			m_buf_map = m_shareId->find<buf_map_type>("MyMap").first;
	}
	else
	{
		m_shareId = new managed_shared_memory(open_or_create, name,65536);
		m_alloc_inst = new void_allocator(m_shareId->get_segment_manager()) ;
		m_buf_map = m_shareId->find<buf_map_type>("MyMap").first;
		if(m_buf_map == NULL )
			m_buf_map = m_shareId->construct<buf_map_type>("MyMap")(std::less<char_string>(), *m_alloc_inst);
	}

}

int CIpcMemBoost::Delete()
{
	//shared_memory_object::remove(name); 
	//delete m_shareId;
	//delete m_alloc_inst;
}

buf_map_type* CIpcMemBoost::GetBuf()
{
	return m_buf_map;
}

int CIpcMemBoost::InsertData(char* apptoken, char*crmtoken,char* crmpwd,int time,int crm_time)
{
 
	char_string p_app(apptoken,*m_alloc_inst);
	buf_map_type::iterator iter = m_buf_map->find(p_app);
	if(iter != m_buf_map->end())
	{
		m_buf_map->erase(iter);
	}
	ShareMemBuf pBuf(crmtoken,crmpwd,*m_alloc_inst,time,crm_time);
	map_value_type value(p_app,pBuf);
	m_buf_map->insert(value);
	if(0 != m_buf_map->size())
		LOG_INFO("buf_map: size[%d] crmtoken[%s]",m_buf_map->size(),m_buf_map->begin()->second.m_crmtoken.c_str());
	return 0; 
 
}

void CIpcMemBoost::DeleteData(char*apptoken,char*crmtoken)
{
	char_string p_app(apptoken,*m_alloc_inst);
	buf_map_type::iterator iter = m_buf_map->find(p_app);
	if(iter != m_buf_map->end())
	{
		memcpy(crmtoken,(iter->second.m_crmtoken).c_str(),(iter->second.m_crmtoken).size());
		m_buf_map->erase(iter);
	}

}

int CIpcMemBoost::UpdateData(char* apptoken, char*crmtoken,char* crmpwd,int time,int crm_time)
{
	int flag = -1;
	char_string p_app(apptoken,*m_alloc_inst);
	buf_map_type::iterator iter = m_buf_map->find(p_app);
	if(iter != m_buf_map->end())
	{
		iter->second.m_lasttime = time;
		iter->second.m_crmtime = crm_time;
		flag = 0;
	}
	else
	{
		ShareMemBuf pBuf(crmtoken,crmpwd,*m_alloc_inst,time,time);
		map_value_type value(p_app,pBuf);
		m_buf_map->insert(value);
	}
	return flag;
}
int CIpcMemBoost::FindCrmtoken(char* apptoken, char* crmtoken)
{
	int flag = -1;
	char_string p_app(apptoken,*m_alloc_inst);
	buf_map_type::iterator iter = m_buf_map->find(p_app);
	if(iter != m_buf_map->end())
	{
		memcpy(crmtoken,iter->second.m_crmtoken.c_str(),iter->second.m_crmtoken.size());
		flag = 0;
	}
	return flag;
}
 

int CIpcMemBoost::FindCrmToken(char* apptoken, char*crmtoken,char* crmpwd,int& time,int& crm_time)
{
	int flag = -1;
	char_string p_app(apptoken,*m_alloc_inst);
	buf_map_type::iterator iter = m_buf_map->find(p_app);
	if(0 != m_buf_map->size())
		LOG_INFO("buf_map: size[%d] crmtoken[%s],apptoken[%s] app_time[%d]",m_buf_map->size(),m_buf_map->begin()->second.m_crmtoken.c_str(), \
			m_buf_map->begin()->first.c_str(),m_buf_map->begin()->second.m_lasttime);
	if(iter != m_buf_map->end())
	{
		memcpy(crmtoken,iter->second.m_crmtoken.c_str(),iter->second.m_crmtoken.size());
		time = iter->second.m_lasttime;
		crm_time = iter->second.m_crmtime;
		memcpy(crmpwd,iter->second.m_crmpwd.c_str(),iter->second.m_crmpwd.size());
		//memcpy(source_id,iter->second.m_companyid.c_str(),iter->second.m_companyid.size());
		flag = 0;
	}
	LOG_INFO("crmtoken[%s] crmpwd[%s] time [%d] crm_time[%d]",crmtoken,crmpwd,time,crm_time);
	return flag;
}
#if 0 
    #include <boost/interprocess/managed_shared_memory.hpp>  
    #include <boost/interprocess/managed_mapped_file.hpp>  
    #include <boost/interprocess/containers/map.hpp>  
    #include <boost/interprocess/allocators/allocator.hpp>  
    #include <functional>  
    #include <cstdlib>  
    #include <utility>  
    #include <sstream>  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <exception>  
    #include <sys/mman.h>  
    #include <sys/stat.h>  
    #include <fcntl.h>    
    #include <string>  
      
    using namespace boost::interprocess;  
    using std::string;  
      
    class Item  
    {  
        public:  
            Item(){}  
            ~Item(){}  
      
            int id;  
            int size;  
            string name;  
    };  
      
    typedef int KeyType;  
    typedef Item MappedType;  
    typedef std::pair<const int, Item> ValueType;  
      
    typedef allocator<ValueType, managed_shared_memory::segment_manager> ShmemAllocator;  
      
    typedef map<KeyType, MappedType, std::less<KeyType>, ShmemAllocator> MyMap;  
      
    int main()  
    {  
        try {  
            // init  
            managed_shared_memory *segment = new managed_shared_memory(create_only, "SharedMemory", 65536);  
      
            const ShmemAllocator *alloc_inst = new ShmemAllocator(segment->get_segment_manager());  
      
            MyMap * mymap = segment.construct<MyMap>("MyMap") (std::less<int>(), alloc_inst);  
      
            Item v;  
            for(int i = 0; i < 100; ++i){  
                v.id = i;  
                mymap->insert(std::pair<const int, Item>(i, (Item)v));  
            }  
      
            for (MyMap::iterator it = mymap->begin(); it != mymap->end(); it++) {  
                printf("%d ", it->second.id);  
            }  
            printf("\n");  
      
            shared_memory_object::remove("SharedMemory");  
        }  
        catch (const std::exception & e) {  
            printf("Exception:%s\n", e.what());  
            shared_memory_object::remove("SharedMemory");  
        }  
      
        return 0;  
      
    }  
#endif

