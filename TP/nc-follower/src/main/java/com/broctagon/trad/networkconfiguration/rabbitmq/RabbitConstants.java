package com.broctagon.trad.networkconfiguration.rabbitmq;

public class RabbitConstants {
		
	public final static String MQ_EXCHANGE_NC = "t_node_grp_t";
	public final static String MQ_EXCHANGE_GRP = "t_node_grp";
	
	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_SYMBOLSPREAD = "symbol_spread_changed";
	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_URLMAP = "sg_ipurl_map_changeed";	
	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_GROUPSPREAD = "group_info_changed";	
	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_BOSYMBOL = "group_info_changed";
	
	// for old tnode version
	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_GROUPSPREAD_OLD = "group_info_changed_old.$";	
	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_BOSYMBOL_OLD = "group_info_changed_old.$";
/*	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_GROUPSPREAD_OLD = "group_info_changed_old";	
	public final static String MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_BOSYMBOL_OLD = "group_info_changed_old";*/
	
	public final static String MQ_QUEUE_NC_FOLLOWER_VERIFIER_DEBUG = "NC.FOLLOWER.verifier.debug.wa";
			
	
	/********************************  tag  **************************************/
	
	public final static short TAG_NC_FOLLOWER_CHNAGENOTICE_SYMBOLSPREAD  = (short) 0x88;
	public final static short TAG_NC_FOLLOWER_CHNAGENOTICE_GROUPSPREAD  = (short) 0x88;
	public final static short TAG_NC_FOLLOWER_CHNAGENOTICE_URLMAP  = (short) 0x88;
	public final static short TAG_NC_FOLLOWER_CHNAGENOTICE_BOSYMBOL  = (short) 0x88;
	
}
