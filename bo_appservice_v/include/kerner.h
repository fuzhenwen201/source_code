#ifndef _KERNER_H_
#define _KERNER_H_

/*核心结构体*/
#include "log.h"
#include "tcp.h"
#include "const.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

typedef struct
{
	string strTel_no;                                         /*电话*/
	string strUser_pwd;
	string strUser_sex;                                       /*性别*/
	string strUser_nick_name;                                 /*昵称*/
	string strUser_qq;                                        /*qq*/
	string strEmail;                                          /*邮箱*/
	string strNet_kind;                                       /*网络环境 M:移动,U:联通,T:电信,W:WIFI,O:其他*/
	string strUser_adr1;                                      /*地址1*/
	string strUser_adr2;                                      /*地址2*/
	string strUser_adr3;                                      /*地址3*/
	string strUser_adr4;                                      /*地址4*/
	string strUser_adr5;                                      /*地址5*/
	string strUser_lst_login_date;                            /*最后登录日期*/
	string strUser_reg_date;                                  /*注册日期*/
	string strUser_log_status;                                /*状态*/
}kerner,*pkerner;

typedef struct 
{
	int nFld_key;                                             /*键值*/
	int nFld_index;                                           /*字段索引*/
	int nFld_tag_len;                                         /*标签长度*/
	string strFld_tag;                                        /*字段标签*/
	int nFld_val_len;                                         /*字段长度*/
	int nFld_fld1;                                            /*备用字段1*/
	int	fld_fld2;                                             /*备用字段2*/
	string strfld_fld3;                                       /*备用字段3*/
	string strfld_fld4;                                       /*备用字段4*/
	string strfld_fld5;                                       /*备用字段5*/
	string strfld_fld6;                                       /*备用字段6*/
}flddef, *pflddef;

#endif
