package cn.fintechstar.traddc.util;

import java.util.Arrays;

public class TNode {

	private int tNodeFlag = 0x009F;
	
	private short tag;       // message tag
	
	private short version;
	
	private String requestId; // 17 bit
	
	private String sessionId; // 17 bit
	
	private String reserved;  // reserved field
	
	private int bodyLength;
	
	private byte[] body;
	
	
	public int gettNodeFlag() {
		return tNodeFlag;
	}

	public void settNodeFlag(int tNodeFlag) {
		this.tNodeFlag = tNodeFlag;
	}

	public short getTag() {
		return tag;
	}

	public void setTag(short tag) {
		this.tag = tag;
	}

	public short getVersion() {
		return version;
	}

	public void setVersion(short version) {
		this.version = version;
	}

	public String getRequestId() {
		return requestId;
	}

	public void setRequestId(String requestId) {
		this.requestId = requestId;
	}

	public String getSessionId() {
		return sessionId;
	}

	public void setSessionId(String sessionId) {
		this.sessionId = sessionId;
	}

	public String getReserved() {
		return reserved;
	}

	public void setReserved(String reserved) {
		this.reserved = reserved;
	}

	public int getBodyLength() {
		return bodyLength;
	}

	public void setBodyLength(int bodyLength) {
		this.bodyLength = bodyLength;
	}

	public byte[] getBody() {
		return body;
	}

	public void setBody(byte[] body) {
		this.body = body;
	}

	@Override
	public String toString() {
		return "TNode [tNodeFlag=" + tNodeFlag + ", tag=" + tag + ", version=" + version + ", requestId=" + requestId
				+ ", sessionId=" + sessionId + ", reserved=" + reserved + ", bodyLength=" + bodyLength + ", body="
				+ Arrays.toString(body) + "]";
	}
	
}
