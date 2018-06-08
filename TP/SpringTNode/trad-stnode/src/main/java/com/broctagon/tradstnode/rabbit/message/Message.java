package com.broctagon.tradstnode.rabbit.message;

import com.broctagon.tradstnode.rabbit.enums.TNodeChannelMsgType;

public class Message implements java.io.Serializable{

	private static final long serialVersionUID = 1L;
	
	public String messageString ;              //The Actual message
    public TNodeChannelMsgType messageType;   //The Type of message
    //public String responceQueue ;            //If it is a service message then queuename should be provided
    public short tag;                   //Tag provided in header
    
    public byte[] getContent(){
    	return messageString.getBytes();
    }
    public void setContent(byte[] content){
    	this.messageString = new String(content);
    }
    
	public String getMessageString() {
		return messageString;
	}
	public void setMessageString(String messageString) {
		this.messageString = messageString;
	}
	public TNodeChannelMsgType getMessageType() {
		return messageType;
	}
	public void setMessageType(TNodeChannelMsgType messageType) {
		this.messageType = messageType;
	}
	/*public String getResponceQueue() {
		return responceQueue;
	}
	public void setResponceQueue(String responceQueue) {
		this.responceQueue = responceQueue;
	}*/
	public short getTag() {
		return tag;
	}
	public void setTag(short tag) {
		this.tag = tag;
	}
     
     
}
