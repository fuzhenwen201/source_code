package com.broctagon.trad.rm.obj.common;

import org.springframework.stereotype.Component;

@Component
public class CommonConstants {

	public final static String MQ_EXCHANGE = "t_node_grp";
	
	public final static short MESSAGE_TAG_USERINFO_RM          = (short) 0x14;
	public final static short MESSAGE_TAG_ORDERANS_RM          = (short) 0x12;
	
	public final static short TAG_GET_TOP5BO_REQ               = (short) 0x0303;    //771;
	public final static short TAG_GET_CUSTOMBO_REQ             = (short) 0x0305;    //773;
	public final static short TAG_GET_SYMBOLBO_REQ             = (short) 0x0307;    //775;
	public final static short TAG_GET_TOP5BO_REMOVE_REQ        = (short) 0x0309;    //777;
	public final static short TAG_GET_CUSTOMBO_REMOVE_REQ      = (short) 0x0311;    //785;
	public final static short TAG_GET_SYMBOLBO_REMOVE_REQ      = (short) 0x0313;    //787;

	public final static short TAG_GET_TOP5BO_ANS               = (short) 0x0304;    //772;
	public final static short TAG_GET_CUSTOMBO_ANS             = (short) 0x0306;    //774;
	public final static short TAG_GET_SYMBOLBO_ANS             = (short) 0x0308;    //776;
	public final static short TAG_GET_TOP5BO_UPDATE_ANS        = (short) 0x030A;    //778; 
	public final static short TAG_GET_CUSTOMBO_UPDATE_ANS      = (short) 0x030C;    //780;
	public final static short TAG_GET_SYMBOLBO_UPDATE_ANS      = (short) 0x030E;    //782;
	
	public final static short TAG_RM_CM_COMMUNICATION_ERQ      = (short) 0x53;
	public final static short TAG_UPDATE_TRADER_BALANCE        = (short) 0x0310;
	public final static short TAG_GET_POSITIONS_REQ_CMT        = (short) 0x0301;		
	public final static short TAG_GET_POSITIONS_ANS_CMT        = (short) 0x0302;	//770
	public final static short TAG_UPDATE_OPEN_TRADE_ONROOT     = (short) 0x031E;
	public final static short TAG_UPDATE_CLOSED_TRADE_ONROOT   = (short) 0x0320;

	public final static short TAG_GET_POSITIONS_REQ            = (short) 0x0323;
	public final static short TAG_GET_POSITIONS_ANS            = (short) 0x0324;		
//	public final static short TAG_UPDATE_POSITION_ONROOT       = (short) 0X0328;     
	public final static short TAG_UPDATE_POSITION_ONROOT       = (short) 0X032C;     //812
	
	public final static short TAG_GET_TRADERS_REQ       	   = (short) 0X0077;     //119
						
	public final static String REDIS_KEY_RM_HASH_USERINFO             =  "RM_HASH_USERINFO";
	public final static String REDIS_KEY_RM_SET_USERINFO              =  "RM_SET_USERINFO";
	public final static String REDIS_KEY_RM_HASH_ORDERANS             =  "RM_HASH_ORDERANS";
	public final static String REDIS_KEY_RM_HASH_OPEN_POSITIONS       =  "RM_HASH_LASTEST_OPEN_POSITIONS";
	public final static String REDIS_KEY_RM_HASH_CLOSE_POSITIONS      =  "RM_HASH_LATEST_CLOSE_POSITIONS";
	public final static String REDIS_KEY_RM_OPENING_POSITION          =  "RM_OPENING_POSITION_REPPORTSERVER";
	public final static String REDIS_KEY_RM_INTRADAY_CLOSED_POSITION  =  "RM_INTRADAY_CLOSED_POSITION";
	
	public final static String REDIS_KEY_RM_SET_ONLINE_USERS		  =  "RM_SET_ONLINE_USERS";
	
	public final static Integer ORDER_STATUS_OPEN		= 0; 
	public final static Integer ORDER_STATUS_WIN		= 136; //(in bit 10001000)
	public final static Integer ORDER_STATUS_LOSS		= 132; //(in bit 10000100)
	public final static Integer ORDER_STATUS_DRAW		= 160; //(in bit 10100000)
	public final static Integer ORDER_STATUS_DEPOSIT 	= 192;
	public final static Integer ORDER_STATUS_WITHDRAW 	= 128;
	
	
	/*
	 * For rm-object-reportserver
	 */
	public final static String MQ_QUEUE_RM_GET_ORDER_REQ 			  = "GETPOSITIONSREQ.RM.ReportServer";
	public final static String MQ_QUEUE_RM_UPDATE_TRADE_BALANCE	      = "RMUPDATETRADERBALANCE.RM";
	public final static String MQ_QUEUE_RM_GETPOSITION_REQ	          = "GETPOSITIONSREQ.RM.Object.ReportServer";
	public final static String MQ_QUEUE_CM_RM_GET_TRADERS 		      = "GETTRADERS.CM.RM.reportserver";
	public final static String MQ_KEY_RM_GET_ORDER_ANS 			      = "GETPOSITIONSANSX.$";
	public final static String MQ_KEY_RM_GET_ORDER_REQ		          = "GETPOSITIONSREQX.RM";
	public final static String MQ_KEY_RM_UPDATE_TRADE_BALANCE         = "RMUPDATETRADERBALANCE.$";
	public final static String MQ_KEY_RM_GETPOSITION_REQ              = "GETPOSITIONSREQ.RM";
	public final static String MQ_KEY_CM_RM_GET_TRADERS               = "GETTRADERS.CM";
	public final static String MQ_QUEUE_RM_GET_ORDER_ANS	          = "GETPOSITIONSANS.ReportServer";
	public final static String MQ_QUEUE_RM_UPDATE_ORDER_ANS	          = "ORDERCHANGEBALANCE.Object.ReportServer";
	public final static String MQ_KEY_RM_UPDATE_ORDER_ANS             = "ORDERCHANGEBALANCE";
	public final static String MQ_ROUTINGKEY_PREFIX_RM_GETPOSITION_ANS= "GETPOSITIONSANS.";
	public final static String MQ_KEY_RM_GET_POSITION_ANS             = "GETPOSITIONSANS.#";
	public final static String MQ_QUEUE_RM_GETPOSITION_ANS            = "GETPOSITIONSANS.RM.Object.ReportServer";
	public final static String MQ_KEY_RM_UPDATE_OPENTRADE_ROOT        = "UPDATEOPENTRADEONROOT.$";
	public final static String MQ_QUEUE_RM_UPDATE_OPENTRADE_ROOT      = "UPDATEOPENTRADEONROOT.Water.Debug";
	public final static String MQ_KEY_RM_UPDATE_CLOSETRADE_ROOT       = "UPDATECLOSEDTRADEONROOT.$";
	public final static String MQ_QUEUE_RM_UPDATE_CLOSETRADE_ROOT     = "UPDATECLOSEDTRADEONROOT.Water.Debug";

	// for TT
	// mq.queue.rm.tt.getposition.req
	public final static String MQ_QUEUE_RM_TT_GETPOSITION_REQ  = "CMTORM";
	// mq.key.rm.tt.getposition.req
	public final static String MQ_KEY_RM_TT_GETPOSITION_REQ    = "CMTORM.Water.Debug";

}
