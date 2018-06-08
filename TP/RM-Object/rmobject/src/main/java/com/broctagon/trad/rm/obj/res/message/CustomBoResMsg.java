package com.broctagon.trad.rm.obj.res.message;

import java.util.List;

import com.broctagon.trad.rm.obj.message.CustomBoMsg;


public class CustomBoResMsg {
	private String reqid;
	private String session;
	private int managerid;
	private int sgid;
	private int direction;
	private List<CustomBoMsg> model;
	
	public List<CustomBoMsg> getModel() {
		return model;
	}
	public void setModel(List<CustomBoMsg> model) {
		this.model = model;
	}
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public String getSession() {
		return session;
	}
	public void setSession(String session) {
		this.session = session;
	}
	public int getSgid() {
		return sgid;
	}
	public void setSgid(int sgid) {
		this.sgid = sgid;
	}
	public int getDirection() {
		return direction;
	}
	public void setDirection(int direction) {
		this.direction = direction;
	}
	
	
}
