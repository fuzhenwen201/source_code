package com.broctagon.trad.rm.db.model;

public class Order {
	
	private String reqid;
	private int groupid;
	private int clientid;
	private String orderid;
	private int orderstatus;
	private int assetid;
	private int direction;
	private long amount;
	private long expiryperiod;
	private long opentime;
	private long openprice;
	private long closeprice;
	private long payout;
	private int sgid;
	private int teid;
	private long expirytime;
	
	// add for change balance order
	private int type;
	private String comment="";
	public int getType() {
		return type;
	}
	public void setType(int type) {
		this.type = type;
	}
	public String getComment() {
		return comment;
	}
	public void setComment(String comment) {
		this.comment = comment;
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
	public int getClientid() {
		return clientid;
	}
	public void setClientid(int clientid) {
		this.clientid = clientid;
	}
	public String getOrderid() {
		return orderid;
	}
	public void setOrderid(String orderid) {
		this.orderid = orderid;
	}
	public int getOrderstatus() {
		return orderstatus;
	}
	public void setOrderstatus(int orderstatus) {
		this.orderstatus = orderstatus;
	}
	public int getAssetid() {
		return assetid;
	}
	public void setAssetid(int assetid) {
		this.assetid = assetid;
	}
	public int getDirection() {
		return direction;
	}
	public void setDirection(int direction) {
		this.direction = direction;
	}
	public long getAmount() {
		return amount;
	}
	public void setAmount(long amount) {
		this.amount = amount;
	}
	public long getExpiryperiod() {
		return expiryperiod;
	}
	public void setExpiryperiod(long expiryperiod) {
		this.expiryperiod = expiryperiod;
	}
	public long getOpentime() {
		return opentime;
	}
	public void setOpentime(long opentime) {
		this.opentime = opentime;
	}
	public long getOpenprice() {
		return openprice;
	}
	public void setOpenprice(long openprice) {
		this.openprice = openprice;
	}
	public long getCloseprice() {
		return closeprice;
	}
	public void setCloseprice(long closeprice) {
		this.closeprice = closeprice;
	}
	public long getPayout() {
		return payout;
	}
	public void setPayout(long payout) {
		this.payout = payout;
	}
	public int getSgid() {
		return sgid;
	}
	public void setSgid(int sgid) {
		this.sgid = sgid;
	}
	public int getTeid() {
		return teid;
	}
	public void setTeid(int teid) {
		this.teid = teid;
	}
	public long getExpirytime() {
		return expirytime;
	}
	public void setExpirytime(long expirytime) {
		this.expirytime = expirytime;
	}
	

}
