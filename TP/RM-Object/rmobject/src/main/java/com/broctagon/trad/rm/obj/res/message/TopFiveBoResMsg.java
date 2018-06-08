package com.broctagon.trad.rm.obj.res.message;

import java.util.List;

import com.broctagon.trad.rm.obj.message.TopFiveBoMsg;

public class TopFiveBoResMsg {
	private String reqid;
	private int managerid;
	private int direction;
	private int sgid;
	private String session;
	private List<TopFiveBoMsg> models;
	
	public List<TopFiveBoMsg> getModels() {
		return models;
	}
	public void setModels(List<TopFiveBoMsg> models) {
		this.models = models;
	}
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
	public int getSgid() {
		return sgid;
	}
	public void setSgid(int sgid) {
		this.sgid = sgid;
	}
	public String getSession() {
		return session;
	}
	public void setSession(String session) {
		this.session = session;
	}
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public int getDirection() {
		return direction;
	}
	public void setDirection(int direction) {
		this.direction = direction;
	}
}
