/**
 * 
 */
package com.broctagon.tradstnode.rabbit.utils;

/**
 * @author kanhaiya
 *
 */
public class ConstantUtils {
	
	/**
	 * Constant belong to write log messages  
	 */
	public static String MESSAGE_IN		= "In";
	public static String MESSAGE_OUT 	= "Out";
	
	public static String EXCHANGE_DECLARATION_NAME				= "t_node_grp";
	public static String EXCHANGE_TYPE_NAME 					= "topic";
	public static String EXCHANGE_NAME_FOR_CM_ADMIN_INTERACTION	= "admin_exc";
	
	
	public static String FILE_PATH_KEY 		= "configFilePath";
	
	public static String CM_START_TIME 		= "cmStartTime";
	public static String ADMIN_START_TIME	= "adminStartTime";
	public static String RM_START_TIME 		= "rmStartTime";
	
	public static String REQUEST_FROM_CM 		= "requestFromCM";
	public static String REQUEST_FROM_ADMIN 	= "requestFromAdmin";
	public static String CM_LOGIN_REQUEST 		= "CM_LOGIN_REQUEST";
	public static String DELETE_MANAGER_REQUEST	= "DELETE_MANAGER_REQUEST";
	
	public static String QUEUE_FOR_USER_DELETE_REQ_CM	= "USERDELETEREQ.CM";
	public static String QUEUE_FOR_CHANGE_BALANCE_CM 	= "CHANGEBALANCE.CM";
	public static String QUEUE_FOR_CREATE_TRADER_CM 	= "CREATETRADER.CM";
	public static String QUEUE_FOR_UPDATE_TRADER_CM 	= "UPDATETRADER.CM";
	public static String QUEUE_FOR_CHANGE_PASSWORD_CM 	= "CHANGEPASSWORD.CM";
	
	// Publisher Queue Name of CM
	public static String PUBLISH_QUEUE_FOR_CM_ECHO_MESSAGE_ANS 			= "MONITOR";
	public static String PUBLISH_QUEUE_FOR_TRADER_LOGIN_ANS 			= "LOGIN";
	public static String PUBLISH_QUEUE_FOR_TRADER_LOGOUT_ANS 			= "LOGOUT";
	public static String PUBLISH_QUEUE_FOR_TRADER_FORCELOGOUT_ANS 		= "FORCELOGOUT";
	public static String PUBLISH_QUEUE_FOR_RM_POSITION					= "CMTORM"; //"RM.POSITION";
	public static String PUBLISH_QUEUE_FOR_PM_PULL_POSITION				= "PULL.POSITION";
	public static String PUBLISH_QUEUE_FOR_TRADER_INFO					= "USERINFO";
	public static String PUBLISH_QUEUE_FOR_TRADER_INFO_TO_RM_REPORT		= "RISKMANAGEMENT.REPORT";  //0x032A (810) & 0x032C (812)
	public static String PUBLISH_QUEUE_FOR_ORDER_ANS					= "ORDERANS";
	public static String PUBLISH_QUEUE_FOR_ORDER_ANS_RM					= "ORDERANS.RM"; // 18
	public static String PUBLISH_QUEUE_FOR_USER_INFO_RM					= "USERINFO.RM"; // 36
	public static String PUBLISH_QUEUE_FOR_ERROR_TO_CLG_POSITION		= "CLG.POSITION";
	public static String PUBLISH_ONLINE_TRADER_ON_ROOT 					= "ONLINETRADERONROOT.$";
	public static String PUBLISH_LOGOUT_TRADER_ON_ROOT 					= "LOGOUTTRADERONROOT.$";
	public static String PUBLISH_NEW_TRADERE_ON_ROOT 					= "NEWTRADERONROOT.$";
	public static String PUBLISH_UPDATE_TRADERE_ON_ROOT 				= "UPDATETRADERONROOT.$";
	public static String PUBLISH_DELETE_TRADERE_ON_ROOT 				= "DELETETRADERONROOT.$";
	public static String PUBLISH_CHANGE_BALANCE_ON_ROOT					= "CHANGEBALANCE.$";
	public static String PUBLISH_QUEUE_FOR_TRADER_TRADING_CONDITION		= "TRADECONDITIONS";
	public static String PUBLISH_QUEUE_OF_CM_TO_ADMIN					= "cm2admin";
	
	public static String PUBLISH_QUEUE_FOR_MANAGER_LOGIN_ANS 			= "LOGINANS";
	public static String PUBLISH_QUEUE_FOR_MANAGER_LOGOUT_ANS 			= "LOGOUTANS";
	public static String PUBLISH_QUEUE_FOR_MANAGER_FORCELOGOUT_ANS 		= "FORCELOGOUT";  //0x007A
	public static String PUBLISH_QUEUE_FOR_GET_MANAGERACCESS_ANS 		= "GETMANAGERACCESSANS";
	public static String PUBLISH_QUEUE_FOR_GET_TRADERS_ANS 				= "GETTRADERSANS";
	public static String PUBLISH_QUEUE_FOR_GET_ONLINETRADER_ANS 		= "GETONLINETRADERANS";
	public static String PUBLISH_QUEUE_FOR_USER_DELETE_CHECK_REQ_ANS	= "USERDELETECHECKREQANS";
	public static String PUBLISH_QUEUE_FOR_USER_DELETE_REQ_ANS 			= "USERDELETEREQANS";
	public static String PUBLISH_QUEUE_FOR_CHANGE_BALANCE_ANS 			= "CHANGEBALANCEANS";
	public static String PUBLISH_QUEUE_FOR_CREATE_TRADER_ANS 			= "CREATETRADERANS";
	public static String PUBLISH_QUEUE_FOR_UPDATE_TRADER_ANS 			= "UPDATETRADERANS";
	public static String PUBLISH_QUEUE_FOR_CHANGE_PASSWORD_ANS 			= "CHANGEPASSWORDANS";
	public static String PUBLISH_QUEUE_FOR_CHANGE_PASSWORD_TRADER_ANS 	= "CHANGEPWD";
	public static String PUBLISH_QUEUE_FOR_DISABLE_TRADER_ANS 			= "DISABLETRADERANS"; // 0x008A
	public static String PUBLISH_QUEUE_FOR_KICKOUT_TRADER_ANS 			= "KICKOUTTRADERANS"; // 0x008C
	public static String PUBLISH_QUEUE_FOR_GETTRADER_BYTRADERID_ANS 	= "GETTRADERBYTRADERIDANS"; // 0x008E
	public static String PUBLISH_QUEUE_FOR_GETALLGROUPBYMANAGERID_ANS 	= "GETALLGROUPBYMANAGERIDANS"; // 0x0090
	
	// Publisher Queue Name of AdminService
	public static String PUBLISH_QUEUE_FOR_ADMIN_ECHO_MESSAGE_ANS 	= "MONITOR";
	public static String PUBLISH_QUEUE_FOR_ADMIN_LOGIN_ANS 			= "ADMINLOGINANS";
	public static String PUBLISH_QUEUE_FOR_ADMIN_LOGOUT_ANS 		= "ADMINLOGOUTANS";
	public static String PUBLISH_QUEUE_FOR_ADMIN_FORCELOGOUT_ANS 	= "ADFORCELOGOUT"; // 0x047A (1146)
	public static String PUBLISH_QUEUE_FOR_ADMIN_CHANGEPASSWORD_ANS = "ADMINCHANGEPASSWORDANS";
	public static String PUBLISH_QUEUE_FOR_CREATE_MANAGER_ANS 		= "CREATEMANAGERANS";
	public static String PUBLISH_QUEUE_FOR_UPDATE_MANAGER_ANS		= "UPDATEMANAGERANS";
	public static String PUBLISH_QUEUE_FOR_DELETE_MANAGER_ANS 		= "DELETEMANAGERANS";
	public static String PUBLISH_QUEUE_FOR_UPDATE_MANAGERRIGHT_ANS	= "UPDATEMANAGERRIGHTANS";
	public static String PUBLISH_QUEUE_FOR_GET_MANAGERRIGHT_ANS 	= "GETMANAGERRIGHTANS";
	public static String PUBLISH_QUEUE_FOR_GET_MANAGERS_ANS 		= "GETMANAGERSANS";
	public static String PUBLISH_QUEUE_FOR_CREATE_GROUP_ANS         = "CREATEGROUPANS";
	public static String PUBLISH_QUEUE_FOR_UPDATE_GROUP_ANS         = "UPDATEGROUPANS";
	public static String PUBLISH_QUEUE_FOR_DELETE_GROUP_ANS         = "DELETEGROUPANS";
	public static String PUBLISH_QUEUE_FOR_GETALL_GROUP_ANS         = "GETALLGROUPANS";
	public static String PUBLISH_QUEUE_FOR_GETALLBYID_GROUP_ANS     = "GETGROUPDETAILSBYIDANS";
	public static String PUBLISH_QUEUE_FOR_GETALLGW_GROUP_ANS       = "GETALLGROUPFORGATEWAYANS";
	public static String PUBLISH_QUEUE_FOR_CREATE_SECURITY_ANS      = "CREATESECURITYANS";
	public static String PUBLISH_QUEUE_FOR_UPDATE_SECURITY_ANS      = "UPDATESECURITYANS";
	public static String PUBLISH_QUEUE_FOR_DELETE_SECURITY_ANS      = "DELETESECURITYANS";
	public static String PUBLISH_QUEUE_FOR_GETALL_SECURITY_ANS      = "GETALLSECURITYANS";
    public static String PUBLISH_QUEUE_FOR_CREATESYMBOL_ANS	        = "CREATESYMBOLANS";
    public static String PUBLISH_QUEUE_FOR_UPDATESYMBOL_ANS         = "UPDATESYMBOLANS";
    public static String PUBLISH_QUEUE_FOR_GETALLSYMBOL_ANS         = "GETALLSYMBOLANS";
    public static String PUBLISH_QUEUE_FOR_GETSYMBOLBYID_ANS        = "GETSYMBOLBYIDANS";
    public static String PUBLISH_QUEUE_FOR_DELETESYMBOL_ANS         = "DELETESYMBOLANS";
	public static String PUBLISH_QUEUE_FOR_CREATE_ACCOUNTS_ANS      = "CREATEACCOUNTANS"; 
    public static String PUBLISH_QUEUE_FOR_UPDATE_ACCOUNTS_ANS      = "UPDATEACCOUNTANS";
    public static String PUBLISH_QUEUE_FOR_GET_ALLACCOUNTS_ANS      = "GETALLACCOUNTSANS";
    public static String PUBLISH_QUEUE_FOR_GETACCOUNTS_BY_ID_ANS    = "GETACCOUNTSBYACCOUNTIDANS";
	public static String PUBLISH_QUEUE_FOR_ENABLE_DISABLE_ANS 		= "ENABLEDISABLEMANAGERANS";
	public static String PUBLISH_QUEUE_FOR_GET_ADMINS_ANS 			= "GETADMINSANS";
	public static String PUBLISH_QUEUE_FOR_DELETE_ADMIN_ANS 		= "DELETEADMINANS";
	public static String PUBLISH_QUEUE_FOR_CREATE_ADMIN_ANS 		= "CREATEADMINANS";
	public static String PUBLISH_QUEUE_FOR_UPDATE_ADMIN_ANS 		= "UPDATEADMINANS";
	public static String PUBLISH_QUEUE_FOR_SYNC_SQLITE              = "ad.config.changed";
	public static String PUBLISH_QUEUE_FOR_SAVE_MANAGERAGAINSTGROUP_ANS ="SAVEMANAGERAGAINSTGROUPSANS";
	public static String PUBLISH_QUEUE_FOR_GET_ALLGROUP_ByMANAGERID_ANS = "GETALLGROUPSBYMANAGERIDANS";
	public static String PUBLISH_QUEUE_SYNCH_DATA_ANS 			    = "SYNCHDATAANS";
	
	public static String PUBLISH_QUEUE_OF_ADMIN_TO_CM				= "admin2cm";
	
	//(MGSG->AD)FORCELOGOUT.ADT = 0x0479 ; (AD->MGSG)ADFORCELOGOUT.[SGID] = 0x047A
	
	
	public static Integer DEPOSIT_OPERATION 			= 0;
	public static Integer WITHDRAW_OPERATION 			= 1;
	public static Integer ORDER_STATUS_FOR_DEPOSIT		= 192;
	public static Integer ORDER_STATUS_FOR_WITHDRAW		= 128;
	public static Integer TRADE_CONDITIONS_TYPE			= 1;

	public static Integer FOR_DELETE_MANAGER_KICK_OUT_CODE	= 401;
	public static Integer FORCELOGOUT_ERROR_CODE			= 501;
	public static Integer KICKOUT_ERROR_CODE				= 502;
	public static Integer DISABLE_ERROR_CODE				= 503;
	public static Integer PERMISSION_DENIED_ERROR_CODE		= 550;
	
	public static Integer SERVER_ERROR_CODE						= 601;
	public static Integer PM_RESPONSE_INCORRECT_ERROR_CODE		= 602;
	public static Integer ACCOUNT_DISBALED_ERROR_CODE			= 603;
	public static Integer BALANCE_INCORRECT_FOUND_ERROR_CODE	= 604;
	public static Integer INCORRECT_LOGIN_PASSWORD_ERROR_CODE	= 605;
	public static Integer RECONNECT_SESSION_EXPIRED_ERROR_CODE	= 606;
	
	
	public static Integer PM_SYNC_TYPE_DURING_LOGIN 					= 1;
	public static Integer PM_SYNC_TYPE_DURING_DEPOSIT 					= 2;
	public static Integer PM_SYNC_TYPE_DURING_WITHDRAW					= 4;
	public static Integer PM_SYNC_TYPE_DURING_FEEDBACK 					= 8; // this is temporarily for reconnecting the trader 
	public static Integer PM_SYNC_TYPE_DURING_SEND_USER_INFO 			= 16;
	public static Integer PM_SYNC_TYPE_DURING_LOGOUT_AND_FORCELOGOUT	= 32;
	
	public static String DEPOSIT_OPERATION_TXT 					= "DEPOSIT";
	public static String WITHDRAW_OPERATION_TXT 				= "WITHDRAW";
	public static String REQUEST_TYPE_TRADER_DETAIL				= "TRADER_DETAIL";
	public static String REQUEST_TYPE_DUPLICATE_TRADER_LOGIN	= "DUPLICATE_LOGIN";
	
	public static Integer ORDER_STATUS_WIN		= 136; //(in bit 10001000)
	public static Integer ORDER_STATUS_LOSS		= 132; //(in bit 10000100)
	public static Integer ORDER_STATUS_DRAW		= 160; //(in bit 10100000)
	
	
}
