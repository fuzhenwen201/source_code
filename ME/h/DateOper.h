#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream> 
//using namespace std;
class DateOper
{
public:
	DateOper(int year = 1900, int mouth = 1, int day = 1);//构造函数 
	bool operator == (const DateOper& d);//日期比较符号的重载 
	bool operator <(const DateOper& d);
	bool operator <=(const DateOper& d);
	bool operator >(const DateOper& d);
	bool operator >=(const DateOper& d);
	bool operator !=(const DateOper& d);
	DateOper operator+ (int day);//运算符的重载 
	DateOper operator+= (int day);
	DateOper operator- (int day);
	DateOper operator-= (int day);
	DateOper operator++();
	DateOper operator++(int);
	DateOper operator--();
	DateOper operator--(int);
	void display();//打印函数，野可以重载cout 
	int operator-(const DateOper& d);//
	int GetYear() { return _year; }
	int GetMonth() { return _mouth;}
	int GetDay() { return _day; }
private:
	bool IsLeapYear(int year);//判断闰年的私有类函数 
	int GetMonthDay(int year, int month);//获取月份的私有类函数 
	int _year;
	int _mouth;
	int _day;
};