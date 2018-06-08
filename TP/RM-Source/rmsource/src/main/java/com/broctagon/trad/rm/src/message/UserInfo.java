package com.broctagon.trad.rm.src.message;

public class UserInfo {

//	{"reqid":"221a9c07734b11e7ae471bcf4ab74159","sessionid":"e9e11e74734a11e7bf8e835496a721bc","groupid":0,
//		"clientid":1,"accountid":0,"prevbalance":0,"changed":10000000,"margin":0,"freemargin":9999540,
//		"profit":-460,"type":128,"closebalance":0,"status":1,"errmsg":"success"}
	
	private String reqid;
	private String sessionid;
	private int groupid;
	private int clientid;
	private int accountid;
	private int prevbalance;
	private long changed;
	private int margin;
	private int freemargin; 
	private int profit;
	private int type;
	private int closebalance;
	private int status;
	private String errmsg;
	
	private int sgid;	
	private int balance;
		
	@Override
	public String toString() {
		return "UserInfo [reqid=" + reqid + ", sessionid=" + sessionid + ", groupid=" + groupid + ", clientid="
				+ clientid + ", accountid=" + accountid + ", prevbalance=" + prevbalance + ", changed=" + changed
				+ ", margin=" + margin + ", freemargin=" + freemargin + ", profit=" + profit + ", type=" + type
				+ ", closebalance=" + closebalance + ", status=" + status + ", errmsg=" + errmsg + "]";
	}
	
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public String getSessionid() {
		return sessionid;
	}
	public void setSessionid(String sessionid) {
		this.sessionid = sessionid;
	}
	public int getGroupid() {
		return groupid;
	}
	public void setGroupid(int groupid) {
		this.groupid = groupid;
	}
	public int getClientid() {
		return clientid;
	}
	public void setClientid(int clientid) {
		this.clientid = clientid;
	}
	public int getAccountid() {
		return accountid;
	}
	public void setAccountid(int accountid) {
		this.accountid = accountid;
	}
	public int getPrevbalance() {
		return prevbalance;
	}
	public void setPrevbalance(int prevbalance) {
		this.prevbalance = prevbalance;
	}
	public long getChanged() {
		return changed;
	}
	public void setChanged(long changed) {
		this.changed = changed;
	}
	public int getMargin() {
		return margin;
	}
	public void setMargin(int margin) {
		this.margin = margin;
	}
	public int getFreemargin() {
		return freemargin;
	}
	public void setFreemargin(int freemargin) {
		this.freemargin = freemargin;
	}
	public int getProfit() {
		return profit;
	}
	public void setProfit(int profit) {
		this.profit = profit;
	}
	public int getType() {
		return type;
	}
	public void setType(int type) {
		this.type = type;
	}
	public int getClosebalance() {
		return closebalance;
	}
	public void setClosebalance(int closebalance) {
		this.closebalance = closebalance;
	}
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	public String getErrmsg() {
		return errmsg;
	}
	public void setErrmsg(String errmsg) {
		this.errmsg = errmsg;
	}
	
	public int getSgid() {
		return sgid;
	}
	public void setSgid(int sgid) {
		this.sgid = sgid;
	}
	public int getBalance() {
		return balance;
	}
	public void setBalance(int balance) {
		this.balance = balance;
	}
	
}
