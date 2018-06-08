/*************************************************************************
	> File Name: regex_abs.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 22 Jun 2017 03:37:45 PM CST
 ************************************************************************/
#ifndef POSITIONMANAGEMENT_REGEX_ABS_H_
#define POSITIONMANAGEMENT_REGEX_ABS_H_

#include<tuple>

template<typename...Args>
class RegExAbs {
	public:
		RegExAbs() = default;
		virtual ~RegExAbs() = default;

		RegExAbs(const RegExAbs& r) = delete;
		RegExAbs& operator=(const RegExAbs& r) = delete;
		RegExAbs(RegExAbs& r) = default;
		RegExAbs& operator=(RegExAbs&& r) = default;

	public:
		virtual int RegExec(char* src) = 0;

		const std::tuple<Args...>& GetTuple(void) const {
			return m_tupleData;
		}

	protected:
		std::tuple<Args...> m_tupleData;
};

#endif

