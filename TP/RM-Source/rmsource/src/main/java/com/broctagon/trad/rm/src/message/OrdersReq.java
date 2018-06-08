package com.broctagon.trad.rm.src.message;

public class OrdersReq {

//	{"session":"ceaf6bf2634011e7aaf71b925476a83c","managerid":1,"reqid":"0cc458dac629438ba7d64cc28c55c3e1","typesoftrade":0,"clientid":0}
	
	private String session;
	private int managerid;	
	private String reqid;
	private int typesoftrade;
	private int clientid;
	
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

}
