package com.broctagon.trad.rm.src.common;

public class CommonConstants {

	// public static String PUBLISH_QUEUE_FOR_GET_ONLINETRADER_ANS = "GETONLINETRADERANS";
	
	public final static String MQ_KEY_GET_ONLINETRADER_REQ            = "GETONLINETRADER.CM";
	public final static String MQ_BINDINGKEY_GET_ONLINETRADER_ANS     = "GETONLINETRADERANS.#";
	public final static String PUBLISH_QUEUE_FOR_GET_ONLINETRADER_ANS = "GETONLINETRADERANS";
	public final static String MQ_BINDINGKEY_TRADER_LOGIN_ANS         = "LOGIN.#";
	public final static String MQ_BINDINGKEY_TRADER_LOGOUT_ANS        = "LOGOUT.#";
	public final static String MQ_BINDINGKEY_TRADER_FORCELOGOUT_ANS   = "FORCELOGOUT.#";

	public final static short TAG_TRADER_LOGIN_ANS           = (short) 0x22; // 34
	public final static short TAG_TRADER_LOGOUT_ANS          = (short) 0x32; // 50
	public final static short TAG_TRADER_FORCELOGOUT_ANS     = (short) 0x52; // 82
	public final static short TAG_CHANGE_BALANCE             = (short) 0x006c;
	public final static short TAG_ONLINE_USERINFO            = (short) 0x24; // Dec 36
	public final static short TAG_OFFLINE_USERINFO           = (short) 0x032A; // Dec 810
	public final static short TAG_USERINFO                   = (short) 0x14;
	public final static short TAG_UPDATE_TRADER_BALANCE      = (short) 0x0310;
	public final static short TAG_GET_ONLINE_TRADERS_REQ     = (short) 0x0081; // Dec 129
	public final static short TAG_GET_ONLINE_TRADERS_ANS     = (short) 0x0082; // Dec 130
	public final static short TAG_TRADER_LOGOUT              = (short) 0x0318; // Dec 792
	public final static short TAG_ORDERANS                   = (short) 0x12;
	public final static short TAG_CHANGE_BALANCE_ORDER       = (short) 0x032C; // Dec 812
																			

	public final static String REDIS_KEY_RM_HASH_USERINFO          = "RM_HASH_USERINFO";
	public final static String REDIS_KEY_RM_SET_USERINFO           = "RM_SET_USERINFO";
	public final static String REDIS_KEY_RM_HASH_ORDERANS          = "RM_HASH_ORDERANS";
	public final static String REDIS_KEY_RM_HASH_UPDATE_USERINFO   = "RM_HASH_UPDATE_USERINFO";
	public final static String REDIS_KEY_RM_SET_UPDATE_USERINFO    = "RM_SET_UPDATE_USERINFO";
	public final static String REDIS_KEY_RM_HASH_CLOSED_ORDERANS   = "RM_HASH_CLOSED_ORDERANS";
	public final static String REDIS_KEY_RM_HASH_OPENUP_ORDERANS   = "RM_HASH_OPENUP_ORDERANS";
	public final static String REDIS_KEY_RM_HASH_OPENDOWN_ORDERANS = "RM_HASH_OPENDOWN_ORDERANS";
	public final static String REDIS_KEY_RM_SET_ONLINE_USERS       = "RM_SET_ONLINE_USERS";

	public final static Integer ORDER_STATUS_WIN      = 136; // (in bit 10001000)
	public final static Integer ORDER_STATUS_LOSS     = 132; // (in bit 10000100)
	public final static Integer ORDER_STATUS_DRAW     = 160; // (in bit 10100000)
	public final static Integer ORDER_STATUS_DEPOSIT  = 192;
	public final static Integer ORDER_STATUS_WITHDRAW = 128;

	/*
	 * For rm-source
	 */
	public final static String MQ_BINDINGKEY_USERINFO_RM     = "USERINFO.#";
	public final static String MQ_QUEUE_USERONLINE_BALANCE   = "VALIDATE.RM";
	public final static String MQ_KEY_USERONLINE_BALANCE     = "VALIDATE.RM";
	public final static String MQ_QUEUE_OFFLINE_USERINFO_RM  = "RISKMANAGEMENT.REPORT.RM.ReportServer";
	public final static String MQ_QUEUE_USERINFO_RM          = "USERINFO.RM";
	public final static String MQ_QUEUE_ORDERANS_RM          = "ORDERANS.RM.ReportServer";
	public final static String MQ_KEY_OFFLINE_USERINFO_RM    = "RISKMANAGEMENT.REPORT";
	public final static String MQ_QUEUE_CHANGEBALANCE        = "CHANGEBALANCEANS.SOURCE";
	public final static String MQ_BINDINGKEY_ORDERANS_RM     = "ORDERANS.#";
	// public final static String MQ_QUEUE_ORDERANS_RM       ="ORDERANS.RM.SOURCE.Debug";
	// public final static String MQ_BINDINGKEY_ORDERANS_RM  ="ORDERANS.$";

}
