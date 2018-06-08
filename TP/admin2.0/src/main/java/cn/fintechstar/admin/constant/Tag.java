package cn.fintechstar.admin.constant;

public class Tag {

	public static final short LOGIN_REQ = 0x0401;
	public static final short LOGIN_ANS = 0x0402;
	public static final short LOGOUT_REQ = 0x0403;
	public static final short LOGOUT_ANS = 0x0404;
	public static final short FORCE_LOGOUT_REQ = 0x0479;
	public static final short FORCE_LOGOUT_ANS = 0x047A;
	public static final short RECONNECTION_REQ = 0x03FF;
	public static final short RECONNECTION_ANS = 0x0400;
	
	public static final short GET_ADMINS_REQ = 0x040F;
	public static final short GET_ADMINS_ANS = 0x0410;
	public static final short DELETE_ADMIN_REQ = 0x0411;
	public static final short DELETE_ADMIN_ANS = 0x0412;
	public static final short CREATE_ADMIN_REQ = 0x0413;
	public static final short CREATE_ADMIN_ANS = 0x0414;
	public static final short UPDATE_ADMIN_REQ = 0x0415;
	public static final short UPDATE_ADMIN_ANS = 0x0416;
	
	public static final short CHANGE_PWD_REQ = 0x0405;
	public static final short CHANGE_PWD_ANS = 0x0406;
	
	public static final short CREATE_MANAGER_REQ = 0x1031;
	public static final short CREATE_MANAGER_ANS = 0x1032;
	public static final short UPDATE_MANAGER_REQ = 0x1033;
	public static final short UPDATE_MANAGER_ANS = 0x1034;
	public static final short DELETE_MANAGER_REQ = 0x040B;
	public static final short DELETE_MANAGER_ANS = 0x040C;
	public static final short UPDATE_MANAGER_ACCESS_REQ = 0x040D;
	public static final short UPDATE_MANAGER_ACCESS_ANS = 0x040E;
	public static final short GET_MANAGERS_REQ = 0x0475;
	public static final short GET_MANAGERS_ANS = 0x0476;
	public static final short ENABLE_MANAGER_REQ = 0x047B;
	public static final short ENABLE_MANAGER_ANS = 0x047C;
	public static final short GET_MANAGER_RIGHT_REQ = 0x047F;
	public static final short GET_MANAGER_RIGHT_ANS = 0x0480;
	public static final short ADD_MANAGER_TO_GROUP_REQ = 0x0805;
	public static final short ADD_MANAGER_TO_GROUP_ANS = 0x0806;
	public static final short GET_GROUP_BY_MANAGER_ID_REQ = 0x0807;
	public static final short GET_GROUP_BY_MANAGER_ID_ANS = 0x0808;
	
	public static final short CREATE_ACCOUNT_REQ = 0x07CF;
	public static final short CREATE_ACCOUNT_ANS = 0x07D0;
	public static final short UPDATE_ACCOUNT_REQ = 0x07D1;
	public static final short UPDATE_ACCOUNT_ANS = 0x07D2;
	public static final short GET_ACCOUNTS_REQ = 0x07D3;
	public static final short GET_ACCOUNTS_ANS = 0x07D4;
	public static final short GET_ACCOUNT_BY_ID_REQ = 0x07D5;
	public static final short GET_ACCOUNT_BY_ID_ANS = 0x07D6;
	
	public static final short CREATE_GROUP_REQ = 0x07F5;
	public static final short CREATE_GROUP_ANS = 0x07F6;
	public static final short UPDATE_GROUP_REQ = 0x07F7;
	public static final short UPDATE_GROUP_ANS = 0x07F8;
	public static final short GET_GROUP_BY_ID_REQ = 0x07F9;
	public static final short GET_GROUP_BY_ID_ANS = 0x07FA;
	public static final short GET_GROUP_FOR_GATEWAY_REQ = 0x07FB;  // reserved
	public static final short GET_GROUP_FOR_GATEWAY_ANS = 0x07FC;  // reserved
	public static final short GET_GROUPS_REQ = 0x07FD;
	public static final short GET_GROUPS_ANS = 0x07FE;
	public static final short DELETE_GROUP_REQ = 0x07FF;  // reserved
	public static final short DELETE_GROUP_ANS = 0x0780;  // reserved
	
	public static final short CREATE_SECURITY_REQ = 0x0815;
	public static final short CREATE_SECURITY_ANS = 0x0816;
	public static final short UPDATE_SECURITY_REQ = 0x0817;
	public static final short UPDATE_SECURITY_ANS = 0x0818;
	public static final short GET_SECURITIES_REQ = 0x0819;
	public static final short GET_SECURITIES_ANS = 0x081A;
	public static final short DELETE_SECURITY_BY_ID_REQ = 0x081B;
	public static final short DELETE_SECURITY_BY_ID_ANS = 0x081C;
	
	public static final short CREATE_SYMBOL_REQ = 0x0831;
	public static final short CREATE_SYMBOL_ANS = 0x0832;
	public static final short UPDATE_SYMBOL_REQ = 0x0833;
	public static final short UPDATE_SYMBOL_ANS = 0x0834;
	public static final short GET_SYMBOLS_REQ = 0x0835;
	public static final short GET_SYMBOLS_ANS = 0x0836;
	public static final short GET_SYMBOL_BY_ID_REQ = 0x0837;
	public static final short GET_SYMBOL_BY_ID_ANS = 0x0838;
	public static final short DELETE_SYMBOL_BY_ID_REQ = 0x0839;  // reserved
	public static final short DELETE_SYMBOL_BY_ID_ANS = 0x083A;  // reserved
	
	public static final short SYNCH_DATA_REQ = 0x083F;
	public static final short SYNCH_DATA_ANS = 0x0840;

	public static final short SEND_TO_NC_REQ = 0x5020;
	
	
	public static final short SEND_TO_CM_WHEN_DIRTY_BIT = 0x5010;
	
	
	
	
	
	
	
	
	
	
	
}
