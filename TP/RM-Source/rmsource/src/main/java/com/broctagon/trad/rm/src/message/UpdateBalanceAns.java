package com.broctagon.trad.rm.src.message;

public class UpdateBalanceAns {

//	{"operation":0,"accountid":74,"balance":101400,"reqid":"cd1869f71c0b4adda25f60489b3f23fe","status":true,"session":"fa454b08776611e7aaf72c3a896b7d45","sgid":2021,"managerid":1}
	
	private String reqid;
	private int operation;
	private int accountid;
	private int balance;
	private boolean status;
	private String session;
	private int sgid;
	private int managerid;
	
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public int getOperation() {
		return operation;
	}
	public void setOperation(int operation) {
		this.operation = operation;
	}
	public int getAccountid() {
		return accountid;
	}
	public void setAccountid(int accountid) {
		this.accountid = accountid;
	}
	public int getBalance() {
		return balance;
	}
	public void setBalance(int balance) {
		this.balance = balance;
	}
	public boolean isStatus() {
		return status;
	}
	public void setStatus(boolean status) {
		this.status = status;
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
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
	
	
	
}
