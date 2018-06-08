package com.broctagon.trad.reportserver.model;

public class Order {

/*	{"reqid":"4e978baf75d54a5d8bca9c0491f9ff9d","groupid":1,"clientid":82,"orderid":"22922d0e631e11e7aaf7b64ac1597823","orderstatus":0,
		"assetid":1,"direction":0,"amount":100,"expiryperiod":0,"opentime":1499436673826456,"openprice":729000,"closeprice":0,"payout":0,
		"sgid":1,"teid":0,"expirytime":"1499436673871456"
	}*/
	
/*	{"sessionid":"55e23d1a737811e7bf8eb845127a3c96","reqid":"9f487ccf972f41e78b4477abba28b982","groupid":0,"clientid":75,
		"orderid":"5a37d820737811e7bf8ec9b8a6734251","orderstatus":18,"assetid":0,"direction":1,"amount":100,"expiryperiod":5,
		"opentime":0,"openprice":0,"closeprice":0,"payout":0,"sgid":1011,"teid":21}*/
	
	private String reqid;
	private int groupid;
	private int clientid;
	private String orderid;
	private int orderstatus;
	private int assetid;
	private int direction;
	private int amount;
	private int expiryperiod;
	private long opentime;
	private int openprice;
	private int closeprice;
	private int payout;
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
	public int getAmount() {
		return amount;
	}
	public void setAmount(int amount) {
		this.amount = amount;
	}
	public int getExpiryperiod() {
		return expiryperiod;
	}
	public void setExpiryperiod(int expiryperiod) {
		this.expiryperiod = expiryperiod;
	}
	public long getOpentime() {
		return opentime;
	}
	public void setOpentime(long opentime) {
		this.opentime = opentime;
	}
	public int getOpenprice() {
		return openprice;
	}
	public void setOpenprice(int openprice) {
		this.openprice = openprice;
	}
	public int getCloseprice() {
		return closeprice;
	}
	public void setCloseprice(int closeprice) {
		this.closeprice = closeprice;
	}
	public int getPayout() {
		return payout;
	}
	public void setPayout(int payout) {
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
