package com.broctagon.trad.reportserver.common;

public class CommonConstants {

	public final static String KEY_CM_ORDER_ID ="KEY_CM_ORDER_ID";
		
	public static final String REDIS_KEY_REPORT_SERVER_INIT_TRADERS = "RS_INIT_TRADERS";
	public static final String REDIS_KEY_REPORT_SERVER_NEW_TRADERS = "RS_NEW_TRADERS";
	public static final String REDIS_KEY_REPORT_SERVER_UPDATE_TRADERS = "RS_UPDATE_TRADERS";
	public static final String REDIS_KEY_REPORT_SERVER_UPDATE_TRADER_BALANCE = "RS_UPDATE_TRADER_BALANCE";
	public static final String REDIS_KEY_REPORT_SERVER_INIT_POSITIONS = "RS_INIT_POSITIONS";
	public static final String REDIS_KEY_REPORT_SERVER_UPDATE_POSITION = "RS_UPDATE_POSITION";
	
	public static final int REAL_TIME_START = 0x008F;
	public static final int REALTIME_PRICE_REQ_TAG = 0x4001;
	public static final int REALTIME_PRICE_OK_TAG = 0x4002;
	public static final int REALTIME_PRICE_ANS_TAG = 0x4003;
			
	public static final String REDIS_KEY_TRADER_LIST = "traderlist";
	
	
//	SimpleDateFormat df = new SimpleDateFormat(DATA_FORMAT_STR);
	public static final String DATA_FORMAT_STR = "yyyy-MM-dd HH:mm:ss";
	
}
