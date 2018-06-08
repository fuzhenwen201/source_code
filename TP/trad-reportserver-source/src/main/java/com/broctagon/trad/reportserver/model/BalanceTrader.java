package com.broctagon.trad.reportserver.model;

public class BalanceTrader {
			
	private int clientid;
	private int groupid;
	private int margin;
	private int freemargin;
	private int prevbalance;
	private int balance;
		
	@Override
	public String toString() {
		return "TraderBalance [clientid=" + clientid + ", groupid=" + groupid + ", freemargin=" + freemargin
				+ ", prevbalance=" + prevbalance + ", balance=" + balance + "]";
	}
	
	public int getClientid() {
		return clientid;
	}
	public void setClientid(int clientid) {
		this.clientid = clientid;
	}
	public int getGroupid() {
		return groupid;
	}
	public void setGroupid(int groupid) {
		this.groupid = groupid;
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
	public int getPrevbalance() {
		return prevbalance;
	}
	public void setPrevbalance(int prevbalance) {
		this.prevbalance = prevbalance;
	}
	public int getBalance() {
		return balance;
	}
	public void setBalance(int balance) {
		this.balance = balance;
	}
		
}
