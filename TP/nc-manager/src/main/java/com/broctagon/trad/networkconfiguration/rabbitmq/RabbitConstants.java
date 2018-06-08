package com.broctagon.trad.networkconfiguration.rabbitmq;

public class RabbitConstants {
		
	public final static String MQ_EXCHANGE = "t_node_grp";
	
	public final static String MQ_BINDINGKEY_NC_MANAGER_NOTICE_ANS    = "ad.config.changed";		
	
	public final static String MQ_QUEUE_NC_MANAGER_CHANGENOTICE_COMMON = "CHANGENOTICE.COMMON.NC.MANAGE";
		
		
	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_SYMBOLSPREAD = "config.symbolspread.changed";
	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_URLMAP = "config.sg.ipurl.map.changed";	
	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_GROUPSPREAD = "config.groupspread.changed";	
	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_BOSYMBOL = "config.bosymbol.changed";
	
	
	/********************************  tag  **************************************/	
	public final static short TAG_NC_MANAGER_CHNAGENOTICE		  	  	  = (short) 0x5020;   // 
	
	public final static short TAG_NC_MANAGER_CHNAGENOTICE_SYMBOLSPREAD    = (short) 0x5010;   // 
	public final static short TAG_NC_MANAGER_CHNAGENOTICE_GROUPFX		  = (short) 0x5020;   // 
	public final static short TAG_NC_MANAGER_CHNAGENOTICE_SECURITYFX	  = (short) 0x5030;   // 
	public final static short TAG_NC_MANAGER_CHNAGENOTICE_GROUPBO         = (short) 0x5040;   // 
	public final static short TAG_NC_MANAGER_CHNAGENOTICE_SECURITYBO      = (short) 0x5050;   // 

}
