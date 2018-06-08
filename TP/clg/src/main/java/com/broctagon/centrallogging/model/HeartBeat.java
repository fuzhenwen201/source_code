package com.broctagon.centrallogging.model;

import java.io.Serializable;
import java.math.BigDecimal;

public class HeartBeat implements Serializable{
	
	private String reqid;
	
	private int mid;
	
	private BigDecimal timestamp;

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

	public BigDecimal getTimestamp() {
		return timestamp;
	}

	public void setTimestamp(BigDecimal timestamp) {
		this.timestamp = timestamp;
	}
	
}
