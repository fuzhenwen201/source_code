#include <iostream> 
#include "DateOper.h" 
//using namespace std;
void DateOper::display()
{
	std::cout << _year << " " << _mouth << " "
		<< _day << std::endl;
}
DateOper::DateOper(int year, int mouth, int day)
{
	if ((year >= 0) &&
		(mouth > 0 && mouth<13) &&
		(day>0 && day <= GetMonthDay(year, mouth)))
	{
		_year = year;
		_mouth = mouth;
		_day = day;
	}
	else
	{
		std::cout << "日期错误，改为默认日期" << std::endl;
		_year = 1900;
		_mouth = 1;
		_day = 1;
	}
}
bool DateOper:: operator == (const DateOper& d)
{
	return this->_year == d._year
		&& this->_mouth == d._mouth
		&& this->_day == d._day;
}
bool DateOper:: operator <(const DateOper& d)
{
	if (_year < d._year)
		return true;
	if (_year == d._year)
	{
		if (_mouth == d._mouth)
		{
			if (_day == d._day)
			{
				return false;
			}
			else if (_day < d._day)
				return true;
		}
		else if (_mouth < d._mouth)
			return true;
	}
	return false;
}
bool DateOper:: operator <=(const DateOper& d)
{
	if (*this<d || *this == d)
		return true;
	return false;
}
bool DateOper:: operator >(const DateOper& d)
{
	if (!(*this <= d))
		return true;
	return false;
}
bool DateOper:: operator >=(const DateOper& d)
{
	if (!(*this < d))
		return true;
	return false;
}
bool DateOper:: operator !=(const DateOper& d)
{
	return !(*this == d);
}
DateOper DateOper:: operator+ (int day)//日期加天数，因为定义的天数为int型，所以可能加一个负的天数 
{  //故在循环中添加了判断处理，若负责执行减操作，反之执行加操作 
	DateOper DestDate(*this);
	DestDate._day += day;
	while (DestDate._day > GetMonthDay(DestDate._year, DestDate._mouth) ||
		DestDate._day <= 0)
	{
		if (DestDate._day>0)
		{
			DestDate._day -= GetMonthDay(DestDate._year, DestDate._mouth);
			if (DestDate._mouth == 12)
			{
				DestDate._mouth = 0;
				DestDate._year++;
			}
			DestDate._mouth++;
		}
		else
		{
			DestDate._day += GetMonthDay(DestDate._year, DestDate._mouth);
			if (DestDate._mouth == 1)
			{
				DestDate._mouth = 13;
				DestDate._year--;
			}
			DestDate._mouth--;
		}
	}
	return DestDate;
}
DateOper DateOper:: operator+= (int day)
{
	*this = *this + day;
	return *this;
}
DateOper DateOper:: operator- (int day)
{
	day = -day;
	return *this + day;
}
DateOper DateOper:: operator-= (int day)
{
	*this = *this - day;
	return *this;
}
DateOper DateOper:: operator++()
{
	DateOper DestDate(*this);
	*this += 1;
	return DestDate;
}
DateOper DateOper:: operator++(int)
{
	*this += 1;
	return *this;
}
DateOper DateOper:: operator--()
{
	DateOper Destdate(*this);
	--*this;
	return Destdate;
}
DateOper DateOper:: operator--(int)
{
	*this -= 1;
	return *this;
}
int DateOper:: operator-(const DateOper& d)//日期-日期的实现主要先比较日期的大小，再用计数器 
{  //统计小日期加多少天月大日期变为同一日期，最后输出结果 
	DateOper high(d);
	DateOper low;
	int count = 0;
	int flag = 1;
	if (*this > d)
	{
		low = high;
		high = *this;
	}
	else
	{
		flag = 0;
		low = *this;
	}
	while (low != high)
	{
		low++;
		count++;
	}
	if (!flag)
		count = -count;
	return count;
}
bool DateOper::IsLeapYear(int year)
{
	if (((year % 4 == 0) && (year % 100)) ||
		(year % 400 == 0))
	{
		return true;
	}
	return false;
}
int DateOper::GetMonthDay(int year, int month)//用一个数组存储1-12月所占天数 
{
	int monthArray[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int day = monthArray[month];
	if (month == 2 && IsLeapYear(year))
	{
		day += 1;
	}
	return day;
}