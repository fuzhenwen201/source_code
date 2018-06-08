package com.broctagon.centrallogging.model;

import java.io.Serializable;
import java.math.BigDecimal;

public class HeartBeatResponse implements Serializable {

	private String reqid;
	
	private int status;
	
	private String name;
	
	private Integer pid;
	
	private Integer mid;
	
	private BigDecimal timestamp;
	
	private BigDecimal starttimes;

	
	public String getReqid() {
		return reqid;
	}

	public void setReqid(String reqid) {
		this.reqid = reqid;
	}

	public int getStatus() {
		return status;
	}

	public void setStatus(int status) {
		this.status = status;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public Integer getPid() {
		return pid;
	}

	public void setPid(Integer pid) {
		this.pid = pid;
	}

	public Integer getMid() {
		return mid;
	}

	public void setMid(Integer mid) {
		this.mid = mid;
	}

	public BigDecimal getTimestamp() {
		return timestamp;
	}

	public void setTimestamp(BigDecimal timestamp) {
		this.timestamp = timestamp;
	}

	public BigDecimal getStarttimes() {
		return starttimes;
	}

	public void setStarttimes(BigDecimal starttimes) {
		this.starttimes = starttimes;
	}
	
}
