package com.broctagon.centrallogging.model;

import java.io.Serializable;

public class Message implements Serializable {

	private static final long serialVersionUID = 1L;

	private String reqid;
	
	private String sessionid;
	 
	private int groupid;
	
	private int clientid;
	
	private int accountid;
	
	private int balance;
	
	private int prevbalance;
	
	private int changed;
	
	private int margin;
	
	private int freemargin;
	
	private int profit;
	
	private int type;
	
	private int closebalance;
	
	private int status;


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


	public int getBalance() {
		return balance;
	}


	public void setBalance(int balance) {
		this.balance = balance;
	}


	public int getPrevbalance() {
		return prevbalance;
	}


	public void setPrevbalance(int prevbalance) {
		this.prevbalance = prevbalance;
	}


	public int getChanged() {
		return changed;
	}


	public void setChanged(int changed) {
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


	@Override
	public String toString() {
		return "{\"reqid\" : " + reqid + ", \"sessionid\" : \"" + sessionid + "\", \"groupid\" : " + groupid + ", \"clientid\" : "
				+ clientid + ", \"accountid\" : " + accountid + ", \"balance\" : " + balance + ", \"prevbalance\" : " + prevbalance
				+ ", \"changed\" : " + changed + ", \"margin\" : " + margin + ", \"freemargin\" : " + freemargin + ", \"profit\" : " + profit
				+ ", \"type\" : " + type + ", \"closebalance\" : " + closebalance + ", \"status\" : " + status + "}";
	}
	
	
}
