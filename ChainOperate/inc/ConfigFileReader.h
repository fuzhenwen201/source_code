/*******************************************************************************************

*Department:	Exchange team

*Decrible:  	简单的读配置文件工具类
				
*Auhor:			Savin

*Createdtime:	2018-02-02

*ModifyTime:	


********************************************************************************************/


#ifndef CONFIGFILEREADER_H_
#define CONFIGFILEREADER_H_

#include <string>
#include <map>

using namespace std;

class CConfigFileReader
{
public:

	int 					LoadFile(const char* filename);

    char* 					GetConfigName(const char* name);

private:
    
    void 					_ParseLine(char* line);
    char*	 				_TrimSpace(char* name);

    bool 					m_load_ok;
    map<string, string> 	m_config_map;
    string 					m_config_file;
};



#endif /* CONFIGFILEREADER_H_ */
