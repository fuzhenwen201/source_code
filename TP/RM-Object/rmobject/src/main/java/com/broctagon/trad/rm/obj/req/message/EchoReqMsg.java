package com.broctagon.trad.rm.obj.req.message;
/**
 * 
 * @author imdadullah
 *
 */
public class EchoReqMsg {
	
	private String reqid;
	private int mid;
	private long timestamp;
	
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public int getMid() {
		return mid;
	}
	public void setMid(int mid) {
		this.mid = mid;
	}
	public long getTimestamp() {
		return timestamp;
	}
	public void setTimestamp(long timestamp) {
		this.timestamp = timestamp;
	}

}

