package com.broctagon.trad.rm.obj.model;

public class BalanceTrader {
			
	private int clientid;
	private int groupid;
	private long margin;
	private long freemargin;
	private long prevbalance;
	private long balance;
		
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

	public long getMargin() {
		return margin;
	}

	public void setMargin(long margin) {
		this.margin = margin;
	}

	public long getFreemargin() {
		return freemargin;
	}

	public void setFreemargin(long freemargin) {
		this.freemargin = freemargin;
	}

	public long getPrevbalance() {
		return prevbalance;
	}

	public void setPrevbalance(long prevbalance) {
		this.prevbalance = prevbalance;
	}

	public long getBalance() {
		return balance;
	}

	public void setBalance(long balance) {
		this.balance = balance;
	}

}
