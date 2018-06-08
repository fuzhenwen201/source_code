/*************************************************************************
  > File Name: xdatetime.h
  > Author: xyz
  > Mail: xiao13149920@foxmail.com 
  > Created Time: Sat 11 Mar 2017 12:58:00 PM CST
 ************************************************************************/
#ifndef XDATETIME_H_
#define XDATETIME_H_

#include<sys/time.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<vector>

template<typename T>
inline bool CountNeg(T data)
{
	return data<= 0;
}
int StringSeparate(std::vector<std::string>& vec, const char* str, const char* delimited);
/****** CCalcTime  begin *****/
class CCalcTime
{
	public:
		CCalcTime();
		~CCalcTime();

		CCalcTime(const CCalcTime& r) = delete;
		CCalcTime& operator=(const CCalcTime& r) = delete;
		CCalcTime(CCalcTime&& r) = delete;
		CCalcTime& operator=(CCalcTime&& r) = delete;
		void Begin(void);
		long End(void);

		/*
		 * 1. time: HH:mm:ss
		 * 2. type: 12 or 24
		 * 3. return: seconds
		 */
		long GetEclipseTime(const char* time=NULL, ...);

	private:
		struct timeval m_start;
		struct timeval m_end;
		long m_time;
};
/****** CCalcTime  end *****/

/* format datetime yyyymmddhhmmss */
/************* CDateTime begin *************/
class CDateTime
{
	public:
		CDateTime(const char* dateTime=NULL, int type=EDATETIMEINT);
		~CDateTime();

		CDateTime(const CDateTime& r) = delete;
		CDateTime& operator=(const CDateTime& r) = delete;
		CDateTime(CDateTime&& r) = delete;
		CDateTime& operator=(CDateTime&& r) = delete;

		enum
		{
			EDATETIME=0x00001, /* yyyy-mm-dd HH:mm:ss */
			EDATETIMEINT=0x00002, /*yyyymmddHHmmss */
			EYEAR=0x00004, /* yyyy */
			EMONTH=0x0008, /* mm:(01, 02, ... 12) */
			EDAY=0x00010, /* dd:(01,02,03...31) */
			EHOUR=0x00020, /* HH:(00,02,03...23) */
			EMINUTE=0x00040, /* ii:(00,02,3...59) */
			ESCONDE=0x00080, /* ss:(0,2,3...59) */
			EUNIXTIMESTAMP=0x00100, /* unix timestamp */
			ETIMEVAL=0x00200,  /*struct timeval */
			ETM=0x00400,  /* struct tm */
		};

	public:
		char* GetDateTime(int type=EDATETIMEINT) const;
		char* GetYestoday(const char* delimiter=NULL) const;
		char* AddDay(int cnt, const char* delimiter=NULL) const;

	protected:
		long long int StrToLong(const char* str, size_t size, const char** end);

	private:
		int  m_year;
		int  m_month;
		int  m_day;
		int  m_hour;
		int  m_minute;
		int  m_second;
		char m_outDay[26];
};
/************* CDateTime end *************/

#endif

