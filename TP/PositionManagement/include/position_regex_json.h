/*************************************************************************
	> File Name: PositionManagement/include/position_regex_json.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Thu 22 Jun 2017 03:44:58 PM CST
 ************************************************************************/
#ifndef POSITIONMANAGEMENT_REGEX_JSON_H_
#define POSITIONMANAGEMENT_REGEX_JSON_H_

#include"regex_abs.h"
#include<regex.h>
#include<error.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>

// constexpr char ORDERANS_FIELDS = 17;
// const char* ORDERANS_REGEX = "^\\{\"sessionid\":\"([0-9a-f]{32})\",\"reqid\":\"([0-9a-f]{32})\",\"groupid\":([0-9]{1,}),\"clientid\":([0-9]{1,}),\"orderid\":\"([0-9a-f]{32})\",\"orderstatus\":([0-9]{1,}),\"assetid\":([0-9]{1,}),\"direction\":([01]{1}),\"amount\":([0-9]{1,}),\"expiryperiod\":([0-9]{1,}),\"opentime\":([0-9]{16}),\"openprice\":([0-9]{1,}),\"closeprice\":([0-9]{1,}),\"payout\":([0-9]{1,}),\"sgid\":([0-9]{1,}),\"teid\":([0-9]{1,})\\}[ \t\r\n]*$";
#define ORDERANS_FIELDS 17
#define ORDERANS_REGEX "^\\{\"sessionid\":\"([0-9a-f]{32})\",\"reqid\":\"([0-9a-f]{32})\",\"groupid\":([0-9]{1,}),\"clientid\":([0-9]{1,}),\"orderid\":\"([0-9a-f]{32})\",\"orderstatus\":([0-9]{1,}),\"assetid\":([0-9]{1,}),\"direction\":([01]{1}),\"amount\":([0-9]{1,}),\"expiryperiod\":([0-9]{1,}),\"opentime\":([0-9]{16}),\"openprice\":([0-9]{1,}),\"closeprice\":([0-9]{1,}),\"payout\":([0-9]{1,}),\"sgid\":([0-9]{1,}),\"teid\":([0-9]{1,})\\}[ \t\r\n]*$"

// constexpr char SENDORDER_FIELDS = 10;
// const char* SENDORDER_REGEX = "^\\{\"reqid\":\"([0-9a-f]{32})\",\"groupid\":([0-9]{1,}),\"clientid\":([0-9]{1,}),\"assetid\":([0-9]{1,}),\"direction\":([01]{1}),\"amount\":([0-9]{1,}),\"sessionid\":\"([0-9a-f]{32})\",\"sgid\":([0-9]{1,})\\}[ \t\r\n]*$";
#define SENDORDER_FIELDS 10
#define SENDORDER_REGEX		"^\\{\"reqid\":\"([0-9a-f]{32})\",\"groupid\":([0-9]{1,}),\"clientid\":([0-9]{1,}),\"assetid\":([0-9]{1,}),\"direction\":([01]{1}),\"expiryperiod\":([0-9]{1,}),\"amount\":([0-9]{1,}),\"sessionid\":\"([0-9a-f]{32})\",\"sgid\":([0-9]{1,})\\}[ \t\r\n]*$"

typedef char* SESSIONID_CSTR;
typedef char* REQID_CSTR;
typedef int	GROUPID_INT;
typedef int	CLIENTID_INT;
typedef int ASSETID_INT;
typedef char DIRECTION_CHAR;
typedef int EXPIRYPERIOD_INT;
typedef int AMOUNT_INT;
typedef int SGID_INT;
typedef char* ORDERID_CSTR;
typedef int ORDERSTATUS_INT;
typedef long OPENTIME_LONG;
typedef int OPENPRICE_INT;
typedef int CLOSEPRICE_INT;
typedef long PAYOUT_LONG;
typedef int TEID_INT;

/**** orderans regex begin *****/
template<typename...Args>
class RegExOrderAns : public RegExAbs<Args...> {
	public:
		RegExOrderAns(void) {
			static_assert(sizeof...(Args) == ORDERANS_FIELDS - 1, "fields have 16");
		}

		RegExOrderAns(RegExOrderAns&& r) = default;
		RegExOrderAns& operator=(RegExOrderAns&& r) = default;
	public:
		static int StaticInit(void) {
			int ret = 0;
			char errmsg[1024];

			if ((ret = regcomp(&re, ORDERANS_REGEX, REG_EXTENDED)) != 0) {
				regerror(ret, &re, errmsg, 1023);
				errmsg[1023] = 0;
				fprintf(stderr, "%s", errmsg);
			}

			return ret;
		}

	public:
		virtual int RegExec(char* src) override;

	private:
		using RegExAbs<Args...>::m_tupleData;
		static regex_t re;
		/* orderans field + 1: because rm[0] store the all match string*/
		regmatch_t rm[ORDERANS_FIELDS]; 

};

template<typename...Args>
regex_t RegExOrderAns<Args...>::re;

template<typename...Args>
int RegExOrderAns<Args...>::RegExec(char* src) {
	int ret = -1;

	ret = regexec(&re, src, SENDORDER_FIELDS, rm, 0);
	if (ret == 0) {
		src[rm[1].rm_eo] = 0;
		src[rm[2].rm_eo] = 0;
		src[rm[3].rm_eo] = 0;
		src[rm[4].rm_eo] = 0;
		src[rm[5].rm_eo] = 0;
		src[rm[6].rm_eo] = 0;
		src[rm[7].rm_eo] = 0;
		src[rm[8].rm_eo] = 0;
		src[rm[9].rm_eo] = 0;
		src[rm[10].rm_eo] = 0;
		src[rm[11].rm_eo] = 0;
		src[rm[12].rm_eo] = 0;
		src[rm[13].rm_eo] = 0;
		src[rm[14].rm_eo] = 0;
		src[rm[15].rm_eo] = 0;
		src[rm[16].rm_eo] = 0;
		std::get<0>(m_tupleData) = &src[rm[1].rm_so];			/* sessionid */
		std::get<1>(m_tupleData) = &src[rm[2].rm_so];			/* reqid */
		std::get<2>(m_tupleData) = atoi(&src[rm[3].rm_so]);		/* groupid */
		std::get<3>(m_tupleData) = atoi(&src[rm[4].rm_so]);		/* clientid */
		std::get<4>(m_tupleData) = &src[rm[5].rm_so];			/* orderid */
		std::get<5>(m_tupleData) = atoi(&src[rm[6].rm_so]);		/* orderstatus */
		std::get<6>(m_tupleData) = atoi(&src[rm[7].rm_so]);		/* assetid */
		std::get<7>(m_tupleData) = atoi(&src[rm[8].rm_so]);		/* direction */
		std::get<8>(m_tupleData) = atoi(&src[rm[9].rm_so]);		/* amount */
		std::get<9>(m_tupleData) = atoi(&src[rm[10].rm_so]);	/*expiryperiod */
		std::get<10>(m_tupleData) = atol(&src[rm[11].rm_so]);	/* opentime */
		std::get<11>(m_tupleData) = atoi(&src[rm[12].rm_so]);	/* openprice */
		std::get<12>(m_tupleData) = atoi(&src[rm[13].rm_so]);	/* closeprice */
		std::get<13>(m_tupleData) = atol(&src[rm[14].rm_so]);	/* payout */
		std::get<14>(m_tupleData) = atoi(&src[rm[15].rm_so]);	/* sgid */
		std::get<15>(m_tupleData) = atoi(&src[rm[16].rm_so]);	/* teid */
	}

	return ret;
}
/**** orderans regex end *****/
template<typename...Args>
class RegExSendOrder : public RegExAbs<Args...> {
	public:
		RegExSendOrder(void) {
			static_assert(sizeof...(Args) == SENDORDER_FIELDS - 1, "fields must have 9");
		}
	public:
		static int StaticInit(void) {
			int ret = 0;
			char errmsg[1024];

			if ((ret = regcomp(&re, SENDORDER_REGEX, REG_EXTENDED)) != 0) {
				regerror(ret, &re, errmsg, 1023);
				errmsg[1023] = 0;
				fprintf(stderr, "%s", errmsg);
			}

			return ret;
		}

	public:
		virtual int RegExec(char* src) override;

	private:
		using RegExAbs<Args...>::m_tupleData;
		static regex_t re;
		/* orderans field + 1: because rm[0] store the all match string*/
		regmatch_t rm[ORDERANS_FIELDS]; 

};

template<typename...Args>
regex_t RegExSendOrder<Args...>::re;

template<typename...Args>
int RegExSendOrder<Args...>::RegExec(char* src) {
	int ret = -1;

	ret = regexec(&re, src, ORDERANS_FIELDS, rm, 0);
	if (ret == 0) {
		src[rm[1].rm_eo] = 0;
		src[rm[2].rm_eo] = 0;
		src[rm[3].rm_eo] = 0;
		src[rm[4].rm_eo] = 0;
		src[rm[5].rm_eo] = 0;
		src[rm[6].rm_eo] = 0;
		src[rm[7].rm_eo] = 0;
		src[rm[8].rm_eo] = 0;
		src[rm[9].rm_eo] = 0;
		std::get<0>(m_tupleData) = &src[rm[1].rm_so];		/* reqid */
		std::get<1>(m_tupleData) = atoi(&src[rm[2].rm_so]);	/* groupid */
		std::get<2>(m_tupleData) = atoi(&src[rm[3].rm_so]);	/* clientid */
		std::get<3>(m_tupleData) = atoi(&src[rm[4].rm_so]);	/* assetid */
		std::get<4>(m_tupleData) = atoi(&src[rm[5].rm_so]);	/* direction */
		std::get<5>(m_tupleData) = atoi(&src[rm[6].rm_so]);	/* expiryperiod */
		std::get<6>(m_tupleData) = atoi(&src[rm[7].rm_so]);	/* amount */
		std::get<7>(m_tupleData) = &src[rm[8].rm_so];		/* sessionid */
		std::get<8>(m_tupleData) = atoi(&src[rm[9].rm_so]);	/* sgid */
	}

	return ret;
}
#endif

