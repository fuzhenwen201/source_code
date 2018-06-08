/**
 * 
 */
package com.broctagon.trad.rm.obj.common;

import org.springframework.stereotype.Component;

/**
 * @author imdadullah
 *
 */
@Component
public class CommonConstants {
	
	public final static String RM_START_TIME 		= "rmStartTime";
	
	/**
	 * Listener Que Name For Getting Request
	 */
		public final static String GET_TOP5BO_REQ = "GETTOP5BOREQ.RM";
		public final static String GET_CUSTOMBO_REQ = "GETCUSTMBOREQ.RM";
		public final static String GET_SYMBOLBO_REQ = "GETSYMBOLBOREQ.RM";
		public final static String GET_POSITION_REQ = "GETPOSITIONSREQ.RM";
		public final static String GET_TOP5BO_REMOVE_REQ = "GETTOP5BOREMOVEREQ.RM";
		public final static String GET_CUSTOMBO_REMOVE_REQ = "GETCUSTMBOREMOVEREQ.RM";
		public final static String GET_SYMBOLBO_REMOVE_REQ = "GETSYMBOLBOREMOVEREQ.RM";
		public final static String GET_USERINFO_REQ = "USERINFO.RM";
		public final static String RM_CM_COMMUNICATION_ERQ="CMTORM";
		public final static String RM_MONITOR="MONITOR.RM";
		
	/**
	 * Response Que Name to Response
	 */
		public final static String GET_TOP5BO_ANS = "GETTOP5BOANS.";
		public final static String GET_CUSTOMBO_ANS = "GETCUSTOMIZEBOANS.";
		public final static String GET_SYMBOLBO_ANS = "GETSYMBOLMONITORANS.";
		public final static String GET_POSITION_ANS = "GETPOSITIONSANS.";
		public final static String GET_TOP5BO_UPDATE_ANS = "RISKMANAGEMENTTOP5BO.$";
		public final static String GET_CUSTOMBO_UPDATE_ANS = "RISKMANAGEMENTCUSTOMBO.$";
		public final static String GET_SYMBOLBO_UPDATE_ANS = "RISKMANAGEMENTSYMBOLBO.$";
		public final static String GET_USERINFO_RES = "RMUPDATETRADERBALANCE.$";
		public final static String GET_UPDATEOPENTRADE_RES = "UPDATEOPENTRADEONROOT.$";
		public final static String GET_UPDATECLOSEDTRADE_RES = "UPDATECLOSEDTRADEONROOT.$";
		public final static String RM_CM_COMMUNICATION_RES="RMTOCM";
		public final static String SENDTRADERPOSITION = "OCPOSITION.";
	//	public final static String SENDTRADERPOSITION = "PULL.POSITION";
		public final static String PUB_CLOSEPOSITION_ADT = "GETRMDATA.ADT";
		public final static String PUB_ECHO_MSG_QUE = "MONITOR.";
	
	
	/**
	 * Listener Tag Name For Getting Request
	 */
		public final static int GET_TOP5BO_REQ_TAG = 771;
		public final static int GET_CUSTOMBO_REQ_TAG = 773;
		public final static int GET_SYMBOLBO_REQ_TAG = 775;
		public final static int GET_POSITION_REQ_TAG = 769;
		public final static int GET_TOP5BO_REMOVE_REQ_TAG = 777;
		public final static int GET_CUSTOMBO_REMOVE_REQ_TAG = 785;
		public final static int GET_SYMBOLBO_REMOVE_REQ_TAG = 787;
		public final static int GET_USERINFO_REQ_TAG = 36;
		public final static int GET_UPDATEOPENTRADE_TAG = 798;
		public final static int GET_UPDATECLOSEDTRADE_TAG = 800;
		public final static int TRADER_HISTORY_REQ_TAG=83;
		
		/**
		 * Response Tag Name to Response
		 */
		public final static int GET_TOP5BO_ANS_TAG = 772;
		public final static int GET_CUSTOMBO_ANS_TAG = 774;
		public final static int GET_SYMBOLBO_ANS_TAG = 776;
		public final static int GET_POSITION_ANS_TAG = 770;
		public final static int GET_TOP5BO_UPDATE_ANS_TAG = 778; 
		public final static int GET_CUSTOMBO_UPDATE_ANS_TAG = 780;
		public final static int GET_SYMBOLBO_UPDATE_ANS_TAG = 782;
		public final static int GET_USERINFO_RES_TAG = 784;
		public final static int PUB_CLOSEPOSITION_ADT_TAG = 1001;
		public final static int PUB_ECHO_MSG_TAG = -24574;
		public final static int TRADER_HISTORY_TAG=42;
		
		/**
		 * Binding Key
		 */
		//mq.key.rm.get.order.ans
		public final static String 	MQ_KEY_RM_GETORDER_ANS="GETPOSITIONSANSX.$";
		//mq.key.rm.update.trade.balance
		public final static String	MQ_KEY_RM_UPDATE_TRADE_BALANCE="RMUPDATETRADERBALANCE.$";
		//mq.key.rm.update.order.ans
		public final static String	MQ_KEY_RM_UPDATE_ORDERANS="ORDERCHANGEBALANCE.$";
		
		/**
		 * FOR REDIS
		 */
		public final static short MESSAGE_TAG_USERINFO_RM = (short) 0x14;
		public final static short MESSAGE_TAG_ORDERANS_RM = (short) 0x12;
		public final static short TAG_GET_POSITIONS_REQ = (short) 0x0323;
		public final static short TAG_GET_POSITIONS_ANS = (short) 0x0324;
		public final static short TAG_UPDATE_TRADER_BALANCE = (short) 0x0326;
		public final static short TAG_UPDATE_POSITION_ONROOT = (short) 0X0328;


		public final static String REDIS_KEY_RM_HASH_ORDERANS = "RM_HASH_ORDERANS";

		public final static String REDIS_KEY_RM_LIST_OPEN_POSITIONS = "RM_LIST_OPEN_POSITIONS";
		public final static String REDIS_KEY_RM_LIST_CLOSE_POSITIONS = "RM_LIST_CLOSE_POSITIONS";

		public final static String REDIS_KEY_RM_HASH_CLOSED_ORDERANS = "RM_HASH_CLOSED_ORDERANS";
		public final static String REDIS_KEY_RM_HASH_OPENUP_ORDERANS = "RM_HASH_OPENUP_ORDERANS";
		public final static String REDIS_KEY_RM_HASH_OPENDOWN_ORDERANS = "RM_HASH_OPENDOWN_ORDERANS";

		public final static String REDIS_KEY_RM_TOP5BO_UP = "RM_HASH_TOP5BO_UP";
		public final static String REDIS_KEY_RM_TOP5BO_DOWN = "RM_HASH_TOP5BO_DOWN";
		public final static String REDIS_KEY_RM_CUSTOMBO_UP = "RM_HASH_CUSTOMBO_UP";
		public final static String REDIS_KEY_RM_CUSTOMBO_DOWN = "RM_HASH_CUSTOMBO_DOWN";
		public final static String REDIS_KEY_RM_SYMBOLBO = "RM_HASH_SYMBOLBO";
}
