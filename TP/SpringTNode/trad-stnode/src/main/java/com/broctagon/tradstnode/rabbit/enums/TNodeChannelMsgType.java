package com.broctagon.tradstnode.rabbit.enums;

import com.broctagon.tradstnode.rabbit.enums.TNodeChannelMsgType;

public enum TNodeChannelMsgType {

	TNODE_EVENT_MSG_TYPE(0), TNODE_SERVICE_MSG_TYPE(1), TNODE_GROUP_MSG_TYPE(2);

	private int value;

	TNodeChannelMsgType(int value) {
		this.value = value;
	}

	public int getValue() {
		return value;
	}

	public static TNodeChannelMsgType getObj(int value){
	        	
		switch(value){   	
		case 0: return TNODE_EVENT_MSG_TYPE;
    	case 1: return TNODE_SERVICE_MSG_TYPE;
    	case 2: return TNODE_GROUP_MSG_TYPE;
    	}
	        	
		throw new IllegalArgumentException("Unknown value: " + value);    	
	}
}
