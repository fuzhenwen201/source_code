package com.broctagon.trad.rm.obj.message;
/**
 * 
 * @author imdadullah
 *
 */
public class UserInfoMsg {

	private String reqid;
	private String sessionid;
	private int groupid;
	private int clientid;
	private int accountid;
	private long prevbalance;
	private long changed;
	private long margin;
	private long freemargin;
	private long profit;
	private int type;
	private long closebalance;
	private int status;
	private long totalbalance;
	
	public long getTotalbalance() {
		return totalbalance;
	}
	public void setTotalbalance(long totalbalance) {
		this.totalbalance = totalbalance;
	}
	public String getSessionid() {
		return sessionid;
	}
	public void setSessionid(String sessionid) {
		this.sessionid = sessionid;
	}
	public int getClientid() {
		return clientid;
	}
	public void setClientid(int clientid) {
		this.clientid = clientid;
	}
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public int getGroupid() {
		return groupid;
	}
	public void setGroupid(int groupid) {
		this.groupid = groupid;
	}
	
	public int getAccountid() {
		return accountid;
	}
	public void setAccountid(int accountid) {
		this.accountid = accountid;
	}
	public long getPrevbalance() {
		return prevbalance;
	}
	public void setPrevbalance(long prevbalance) {
		this.prevbalance = prevbalance;
	}
	public long getChanged() {
		return changed;
	}
	public void setChanged(long changed) {
		this.changed = changed;
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
	public long getProfit() {
		return profit;
	}
	public void setProfit(long profit) {
		this.profit = profit;
	}
	public int getType() {
		return type;
	}
	public void setType(int type) {
		this.type = type;
	}
	public long getClosebalance() {
		return closebalance;
	}
	public void setClosebalance(long closebalance) {
		this.closebalance = closebalance;
	}
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	
}
