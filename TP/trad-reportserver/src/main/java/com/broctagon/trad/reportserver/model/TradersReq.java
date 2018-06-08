package com.broctagon.trad.reportserver.model;

public class TradersReq {
	
	//	{"clientid":0,"managerid":1,"reqid":"7699a6cd942448e1920f49d76ff2cc72",  "session":"4022a1f224c540acb624a728b721162b","sgid":1}
	
	private int clientid;  
	private int managerid;  
	private String reqid; 
	private String session;  
	private int sgid;
	
	public int getClientid() {
		return clientid;
	}
	public void setClientid(int clientid) {
		this.clientid = clientid;
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
	
	
	
}
