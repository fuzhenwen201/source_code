package com.broctagon.trad.rm.db.model;

public class TradeModel {
	private String reqid;
	private int groupid;
	private int clientid;
	private int accountid;
	private int boAccountid;
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
	public long getExpirytime() {
		return expirytime;
	}
	public void setExpirytime(long expirytime) {
		this.expirytime = expirytime;
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
	public int getBoAccountid() {
		return boAccountid;
	}
	public void setBoAccountid(int boAccountid) {
		this.boAccountid = boAccountid;
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
	/* (non-Javadoc)
	 * @see java.lang.Object#hashCode()
	 */
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((orderid == null) ? 0 : orderid.hashCode());
		return result;
	}
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (!(obj instanceof TradeModel))
			return false;
		TradeModel other = (TradeModel) obj;
		if (orderid == null) {
			if (other.orderid != null)
				return false;
		} else if (!orderid.equals(other.orderid))
			return false;
		return true;
	}
}
