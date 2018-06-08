/*************************************************************************
  > File Name: xdatetime.cc
  > Author: xyz
  > Mail: xiao13149920@foxmail.com 
  > Created Time: Sat 11 Mar 2017 12:53:39 PM CST
 ************************************************************************/
#include<algorithm>
#include<time.h>
#include<stdarg.h>
#include"xdatetime.h"
//#include"define.h"

int StringSeparate(std::vector<std::string>& vec, const char* str, const char* delimited)
{
	if (str == NULL)
	{
		return -1;
	}

	char* buf = NULL;

	asprintf(&buf, "%s", str);
	char* token = NULL;

	token = strtok(buf, delimited);
	int ret = 0;

	while (token != NULL)
	{
		vec.push_back(token);
		token = strtok(NULL, delimited);
		++ret;
	}

	free(buf);
	buf = NULL;

	return ret;
}
/****** CCalcTime  begin *****/
CCalcTime::CCalcTime()
{
	m_start.tv_sec = 0;
	m_start.tv_usec = 0;
	m_end.tv_sec = 0;
	m_end.tv_usec = 0;

	m_time = 0;
}

CCalcTime::~CCalcTime()
{
}

void CCalcTime::Begin(void)
{
	gettimeofday(&m_start, NULL);
}

long CCalcTime::End(void)
{
	gettimeofday(&m_end, NULL);

	m_time = (m_end.tv_sec - m_start.tv_sec)*1000000 + (m_end.tv_usec - m_start.tv_usec);

	m_start = m_end;

	return m_time;
}

/* HH:mm:ss */
long CCalcTime::GetEclipseTime(const char* time, ...)
{
	if (NULL == time)
	{
		struct timeval end;
		gettimeofday(&end, NULL);
		m_time = (end.tv_sec - m_start.tv_sec)*1000000 + (end.tv_usec - m_start.tv_usec);

		return m_time/1000000;
	}
	else
	{
		struct tm* nowtm = localtime(&m_start.tv_sec);
		int year = nowtm->tm_year + 1900;
		int month = nowtm->tm_mon + 1;
		int day = nowtm->tm_mday;
		int hour = nowtm->tm_hour;
		int minute = nowtm->tm_min;
		int second = nowtm->tm_sec;

		struct tm tm;
		time_t t;
		tm.tm_year = year - 1900;
		tm.tm_mon = month - 1;
		tm.tm_mday = day;

		std::vector<std::string> vec;
		vec.clear();
		vec.push_back(time);
		va_list argList;
		va_start(argList,time);
		const char* arg = va_arg(argList, const char*);
		while (arg != NULL)
		{
			vec.push_back(arg);
			arg = va_arg(argList, const char*);
		}

		va_end(argList);

		std::vector<int> vHour;
		std::vector<int> vMinute;

		std::vector<std::string>::iterator it = vec.begin();
		std::vector<std::string>::iterator end = vec.end();
		size_t size = vec.size();

		std::vector<long> retVec;
		for (; it != end; ++it)
		{
			std::vector<std::string> vecTmp;
			StringSeparate(vecTmp, it->c_str(), ": ");
			tm.tm_hour = atoi(vecTmp[0].c_str());
			tm.tm_min = atoi(vecTmp[1].c_str());
			tm.tm_sec = atoi(vecTmp[2].c_str());
			t = mktime(&tm);
			retVec.push_back(t-m_start.tv_sec);
		}

		size_t cnt = count_if(retVec.begin(), retVec.end(), CountNeg<long>);
		if (cnt ==retVec.size())
		{
			/*find the min unixtimestamp*/
			t = *std::min_element(retVec.begin(), retVec.end())+24*60*60;
		}
		else
		{
			std::vector<long>::iterator it = retVec.begin();
			std::vector<long>::iterator end = retVec.end();
			t = retVec[0];
			for (; it!=end; ++it)
			{
				if (*it>0)
				{
					if (t <= 0)
					{
						t = *it;
					}
					else if (t > *it)
					{
						t=*it;
					}
				}
			}
		}

		return t;
	}
}
/* CCalcTime  end */

/* format datetime yyyymmddhhmmss */
/************* CDateTime begin *************/
CDateTime::CDateTime(const char* dateTime, int type)
{
	memset(m_outDay, 0, sizeof(m_outDay));
	if (dateTime==NULL)
	{
		/* set currTime to this */
		struct timeval tv;
		gettimeofday(&tv, NULL);
		struct tm* nowtm = localtime(&tv.tv_sec);
		m_year = nowtm->tm_year + 1900;
		m_month = nowtm->tm_mon + 1;
		m_day = nowtm->tm_mday;
		m_hour = nowtm->tm_hour;
		m_minute = nowtm->tm_min;
		m_second = nowtm->tm_sec;
	}
	else if (strncmp(dateTime,"0",1)==0)
	{
		m_year=m_month=m_day=m_hour=m_minute=m_second=0;
	}
	else
	{
		const char* day = dateTime;
		const char* end = NULL;
		while(*day==' ' || *day =='\t')
		{
			++day;
		}

		if (type == EDATETIMEINT)
		{
			m_year = StrToLong(day, 4, &end);
			day = end;
			end = NULL;
			m_month = StrToLong(day, 2, &end);
			day = end;
			end = NULL;
			m_day = StrToLong(day, 2, &end);
			day = end;
			end = NULL;
			m_hour = StrToLong(day, 2, &end);
			day = end;
			end = NULL;
			m_minute = StrToLong(day, 2, &end);
			day = end;
			end = NULL;
			m_second = StrToLong(day, 2, &end);
		}
		else if(type==ETIMEVAL)
		{
			struct timeval* tv=(struct timeval*)dateTime;
			gettimeofday(tv, NULL);
			struct tm* nowtm = localtime(&tv->tv_sec);
			m_year = nowtm->tm_year + 1900;
			m_month = nowtm->tm_mon + 1;
			m_day = nowtm->tm_mday;
			m_hour = nowtm->tm_hour;
			m_minute = nowtm->tm_min;
			m_second = nowtm->tm_sec;
		}
		else if(type==ETM)
		{
			struct tm* nowtm=(struct tm*)dateTime;
			m_year = nowtm->tm_year + 1900;
			m_month = nowtm->tm_mon + 1;
			m_day = nowtm->tm_mday;
			m_hour = nowtm->tm_hour;
			m_minute = nowtm->tm_min;
			m_second = nowtm->tm_sec;
		}
		else if(type==EUNIXTIMESTAMP)
		{
			long day =atol(dateTime);
			struct tm* nowtm = localtime(&day);
			m_year = nowtm->tm_year + 1900;
			m_month = nowtm->tm_mon + 1;
			m_day = nowtm->tm_mday;
			m_hour = nowtm->tm_hour;
			m_minute = nowtm->tm_min;
			m_second = nowtm->tm_sec;
		}
		else if (type == EDATETIME)
		{
			sscanf(dateTime, "%04d-%02d-%02d %02d:%02d:%02d", &m_year, &m_month, &m_day, &m_hour, &m_minute, &m_second);
		}
	}
}

CDateTime::~CDateTime()
{
}

long long int CDateTime::StrToLong(const char* str, size_t size, const char** end)
{
	const char* data = str;
	/* trim blank */
	while(*data==' '||*data=='\t')
	{
		++data;
	}

	long long ret = 0;
	if (size == 0)
	{
		return strtoll(str, NULL, 10);
	}

	while(size && *data != 0)
	{
		ret = 10*ret + *data-48;
		--size;
		++data;
	}
	*end = data;

	return ret;
}

char* CDateTime::GetDateTime(int type) const
{
	char* day =(char*) malloc(26);
	memset(day, 0, sizeof(26));
	if (type & EDATETIME)
	{
		sprintf(day,"%d-%02d-%02d %02d:%02d:%02d", m_year, m_month, m_day, m_hour, m_minute, m_second);

		return day;
	}

	if (type & EDATETIMEINT)
	{
		sprintf(day,"%d%02d%02d%02d%02d%02d", m_year, m_month, m_day, m_hour, m_minute, m_second);

		return day;
	}

	if (type & EUNIXTIMESTAMP)
	{
		struct tm tm;
		time_t t;
		if (m_year==0 && m_month==0 && m_day==0 && m_hour==0 && m_minute==0 && m_second==0)
		{
			sprintf(day, "%s", "0");
		}
		else
		{
			tm.tm_year = m_year - 1900;
			tm.tm_mon = m_month - 1;
			tm.tm_mday = m_day;
			tm.tm_hour = m_hour;
			tm.tm_min = m_minute;
			tm.tm_sec = m_second;
			t = mktime(&tm);
			sprintf(day, "%ld",t);
		}
		return day;
	}

	if(type & EYEAR )
	{
		sprintf(day, "%4d", m_year);
	}

	if (type & EMONTH)
	{
		sprintf(&day[strlen(day)], "%02d", m_month);
	}

	if (type & EDAY)
	{
		sprintf(&day[strlen(day)], "%02d", m_day);
	}

	if (type & EHOUR)
	{
		sprintf(&day[strlen(day)], "%02d", m_hour);
	}

	if(type & EMINUTE)
	{
		sprintf(&day[strlen(day)], "%02d", m_minute);
	}

	if(type & ESCONDE)
	{
		sprintf(&day[strlen(day)], "%02d", m_second);
	}

	return day;
}

/*yyyymmdd */
char* CDateTime::GetYestoday(const char* delimiter) const
{
	char *timestamp = GetDateTime(CDateTime::EUNIXTIMESTAMP);
	long yestoday = atol(timestamp)-24*60*60;
	free(timestamp);
	timestamp = nullptr;

	struct tm* nowtm = localtime(&yestoday);
	char* str = (char*)malloc(21);
	memset(str, 0,sizeof(21));
	if (NULL == delimiter)
	{
		sprintf(str, "%04d%02d%02d", nowtm->tm_year + 1900, nowtm->tm_mon + 1, nowtm->tm_mday);
	}
	else
	{
		sprintf(str, "%04d%s%02d%s%02d", nowtm->tm_year + 1900,delimiter, nowtm->tm_mon + 1, delimiter, nowtm->tm_mday);
	}

	return str;
}

char* CDateTime::AddDay(int cnt, const char* delimiter) const
{
	char *timestamp = GetDateTime(CDateTime::EUNIXTIMESTAMP);
	long day = atol(timestamp) + cnt*24*60*60;
	free(timestamp);
	timestamp = nullptr;

	struct tm* nowtm = localtime(&day);
	char* str =(char*) malloc(21);
	memset(str, 0,sizeof(21));
	if (NULL == delimiter)
	{
		sprintf(str, "%04d%02d%02d", nowtm->tm_year + 1900, nowtm->tm_mon + 1, nowtm->tm_mday);
	}
	else
	{
		sprintf(str, "%04d%s%02d%s%02d", nowtm->tm_year + 1900,delimiter, nowtm->tm_mon + 1, delimiter, nowtm->tm_mday);
	}

	return str;
}
/************* CDateTime end *************/
