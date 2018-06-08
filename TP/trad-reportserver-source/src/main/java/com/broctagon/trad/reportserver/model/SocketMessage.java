package com.broctagon.trad.reportserver.model;

public class SocketMessage {
	
	private int tag;
	private byte[] body;
	
	public int getTag() {
		return tag;
	}
	public void setTag(int tag) {
		this.tag = tag;
	}
	public byte[] getBody() {
		return body;
	}
	public void setBody(byte[] body) {
		this.body = body;
	}

}
