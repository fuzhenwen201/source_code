package com.broctagon.trad.reportserver.model;

public class LoginAns {

//	{"managerid":1,"reqid":"88bf9e22c36844c8a740e05e88ed97cc","status":true,"session":"527e783a794c11e7aaf7587c3a2bd694","sgid":2021}
//	{"managerid":1,"groupid":0,"loginstatus":true,"reqid":"88bf9e22c36844c8a740e05e88ed97cc","session":"e181d6c6794c11e7aaf789537bd2c46a","sgid":2021}
	
	private int managerid;
	private int groupid;
	private boolean loginstatus;
	private String reqid;
	private String session;
	private int sgid;
	
	private boolean status;
	public boolean isStatus() {
		return status;
	}
	public void setStatus(boolean status) {
		this.status = status;
	}
	
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
	public int getGroupid() {
		return groupid;
	}
	public void setGroupid(int groupid) {
		this.groupid = groupid;
	}
	public boolean isLoginstatus() {
		return loginstatus;
	}
	public void setLoginstatus(boolean loginstatus) {
		this.loginstatus = loginstatus;
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

}
