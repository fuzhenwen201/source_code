package com.broctagon.centrallogging.model;

import java.io.Serializable;
import java.math.BigDecimal;

public class ErrorMessage implements Serializable{
	
	private static final long serialVersionUID = 1L;
	
	private Integer tag;
	
	private BigDecimal time;
	
	private String queue;
	
	private String module;
	
	private int errno;
	
	private String errmsg;

	private Message body;
	

	public Integer getTag() {
		return tag;
	}

	public void setTag(Integer tag) {
		this.tag = tag;
	}

	public BigDecimal getTime() {
		return time;
	}

	public void setTime(BigDecimal time) {
		this.time = time;
	}

	public String getQueue() {
		return queue;
	}

	public void setQueue(String queue) {
		this.queue = queue;
	}

	public String getModule() {
		return module;
	}

	public void setModule(String module) {
		this.module = module;
	}

	public int getErrno() {
		return errno;
	}

	public void setErrno(int errno) {
		this.errno = errno;
	}

	public String getErrmsg() {
		return errmsg;
	}

	public void setErrmsg(String errmsg) {
		this.errmsg = errmsg;
	}

	public Message getBody() {
		return body;
	}

	public void setBody(Message body) {
		this.body = body;
	}
	
}
