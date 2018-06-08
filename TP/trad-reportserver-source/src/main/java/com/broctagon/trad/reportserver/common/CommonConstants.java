package com.broctagon.trad.reportserver.common;

public class CommonConstants {
	
	public final static String KEY_CM_ORDER_ID ="KEY_CM_ORDER_ID";
	
	public static final int REAL_TIME_START = 0x008F;
	public static final int REALTIME_PRICE_REQ_TAG = (short)0x4001;
	public static final int REALTIME_PRICE_OK_TAG = (short)0x4002;
	public static final int REALTIME_PRICE_ANS_TAG = (short)0x4003;
	
	public static final short TAG_MGSG_LOGIN_REQ = (short)0x65; 
	public static final short TAG_CM_FORCELOGOUT_ANS = (short)0x007A;
	
	public static final short GET_TRADERS_REQ_TAG = (short)0x77;
	public static final short GET_TRADERS_ANS_TAG = (short)0x78;
	public static final short UPDATE_TRADERS_ANS_TAG = (short)0x031C;
	public static final short NEW_TRADERS_ANS_TAG = (short)0x031A;
	
	public final static short TAG_UPDATE_TRADER_BALANCE = (short)0x0310;
			
//	public static final short GET_ORDER_REQ_TAG = (short)0x0301;
//	public static final short GET_ORDER_ANS_TAG = (short)0x0302;
//	public static final short UPDATE_OPEN_TRADE_TAG = (short)0X031E;
	public static final short UPDATE_CLOSED_TRADE_TAG = (short)0x0320;
	
	public final static short TAG_GET_POSITIONS_REQ = (short)0x0323;
	public final static short TAG_GET_POSITIONS_ANS = (short)0x0324;		
//	public final static short TAG_UPDATE_POSITION_ONROOT = (short)0X0328;
	public final static short TAG_UPDATE_POSITION_ONROOT = (short)0X032C;
				
	public static final String REDIS_KEY_REPORT_SERVER_INIT_TRADERS = "RS_INIT_TRADERS";
	public static final String REDIS_KEY_REPORT_SERVER_NEW_TRADERS = "RS_NEW_TRADERS";
	public static final String REDIS_KEY_REPORT_SERVER_UPDATE_TRADERS = "RS_UPDATE_TRADERS";
	public static final String REDIS_KEY_REPORT_SERVER_UPDATE_TRADER_BALANCE = "RS_UPDATE_TRADER_BALANCE";
	public static final String REDIS_KEY_REPORT_SERVER_INIT_POSITIONS = "RS_INIT_POSITIONS";
	public static final String REDIS_KEY_REPORT_SERVER_UPDATE_POSITION = "RS_UPDATE_POSITION";

	
//	SimpleDateFormat df = new SimpleDateFormat(DATA_FORMAT_STR);
	public static final String DATA_FORMAT_STR = "yyyy-MM-dd HH:mm:ss";
	
}
