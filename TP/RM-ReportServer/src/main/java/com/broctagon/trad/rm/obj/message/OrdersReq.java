package com.broctagon.trad.rm.obj.message;

public class OrdersReq {

//	{"session":"ceaf6bf2634011e7aaf71b925476a83c","managerid":1,"reqid":"0cc458dac629438ba7d64cc28c55c3e1","typesoftrade":0,"clientid":0}
//	{"session":"eeb7d5e48f2b11e7bf8eadc742b39586","managerid":2,"reqid":"48bc0079e4b44781858c4d537331e2aa","typesoftrade":0,"clientid":0,"sgid":2021}
	
	private String session;
	private int managerid;	
	private String reqid;
	private int typesoftrade;
	private int clientid;
	private int sgid;
	
	public String getSession() {
		return session;
	}
	public void setSession(String session) {
		this.session = session;
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
	public int getTypesoftrade() {
		return typesoftrade;
	}
	public void setTypesoftrade(int typesoftrade) {
		this.typesoftrade = typesoftrade;
	}
	public int getClientid() {
		return clientid;
	}
	public void setClientid(int clientid) {
		this.clientid = clientid;
	}
	public int getSgid() {
		return sgid;
	}
	public void setSgid(int sgid) {
		this.sgid = sgid;
	}

}
