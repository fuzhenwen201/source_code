package com.broctagon.trad.reportserver.model;

import java.sql.Timestamp;

public class Trade {

	private Integer ticket;
	private String orderid;
	public String getOrderid() {
		return orderid;
	}

	public void setOrderid(String orderid) {
		this.orderid = orderid;
	}

	private Integer login;
	private String symbol;
	private int digits;
	private int cmd;
	private int volume;
	private Timestamp open_time;
	private double open_price;
	private double sl;
	private double tp;
	private Timestamp close_time;
	private Timestamp expiration;
	private int reason;
	private double conv_rate1;
	private double conv_rate2;
	private double commission;
	private double commission_agent;
	private double swaps;
	private double close_price;
	private double profit;
	private double taxes;
	private String comment;
	private Integer internal_id;
	private double margin_rate;
	private Integer timestamp;
	private int magic;
	private int gw_volume;
	private int gw_open_price;
	private int gw_close_price;
	private Timestamp modify_time;
	
	
	public Integer getTicket() {
		return ticket;
	}
	
	public void setTicket(Integer ticket) {
		this.ticket = ticket;
	}
	
	public Integer getLogin() {
		return login;
	}
	
	public void setLogin(Integer login) {
		this.login = login;
	}
	
	public String getSymbol() {
		return symbol;
	}
	
	public void setSymbol(String symbol) {
		this.symbol = symbol;
	}
	
	public int getDigits() {
		return digits;
	}
	
	public void setDigits(int digits) {
		this.digits = digits;
	}
	
	public int getCmd() {
		return cmd;
	}
	
	public void setCmd(int cmd) {
		this.cmd = cmd;
	}
	
	public int getVolume() {
		return volume;
	}
	
	public void setVolume(int volume) {
		this.volume = volume;
	}
	
	public Timestamp getOpen_time() {
		return open_time;
	}
	
	public void setOpen_time(Timestamp open_time) {
		this.open_time = open_time;
	}
	
	public double getOpen_price() {
		return open_price;
	}
	
	public void setOpen_price(double open_price) {
		this.open_price = open_price;
	}
	
	public double getSl() {
		return sl;
	}
	
	public void setSl(double sl) {
		this.sl = sl;
	}
	
	public double getTp() {
		return tp;
	}
	
	public void setTp(double tp) {
		this.tp = tp;
	}
	
	public Timestamp getClose_time() {
		return close_time;
	}
	
	public void setClose_time(Timestamp close_time) {
		this.close_time = close_time;
	}
	
	public Timestamp getExpiration() {
		return expiration;
	}
	
	public void setExpiration(Timestamp expiration) {
		this.expiration = expiration;
	}
	
	public int getReason() {
		return reason;
	}
	
	public void setReason(int reason) {
		this.reason = reason;
	}
	
	public double getConv_rate1() {
		return conv_rate1;
	}
	
	public void setConv_rate1(double conv_rate1) {
		this.conv_rate1 = conv_rate1;
	}
	
	public double getConv_rate2() {
		return conv_rate2;
	}
	
	public void setConv_rate2(double conv_rate2) {
		this.conv_rate2 = conv_rate2;
	}
	
	public double getCommission() {
		return commission;
	}
	
	public void setCommission(double commission) {
		this.commission = commission;
	}
	
	public double getCommission_agent() {
		return commission_agent;
	}
	
	public void setCommission_agent(double commission_agent) {
		this.commission_agent = commission_agent;
	}
	
	public double getSwaps() {
		return swaps;
	}
	
	public void setSwaps(double swaps) {
		this.swaps = swaps;
	}
	
	public double getClose_price() {
		return close_price;
	}
	
	public void setClose_price(double close_price) {
		this.close_price = close_price;
	}
	
	public double getProfit() {
		return profit;
	}
	
	public void setProfit(double profit) {
		this.profit = profit;
	}
	
	public double getTaxes() {
		return taxes;
	}
	
	public void setTaxes(double taxes) {
		this.taxes = taxes;
	}
	
	public String getComment() {
		return comment;
	}
	
	public void setComment(String comment) {
		this.comment = comment;
	}
	
	public Integer getInternal_id() {
		return internal_id;
	}
	
	public void setInternal_id(Integer internal_id) {
		this.internal_id = internal_id;
	}
	
	public double getMargin_rate() {
		return margin_rate;
	}
	
	public void setMargin_rate(double margin_rate) {
		this.margin_rate = margin_rate;
	}
	
	public Integer getTimestamp() {
		return timestamp;
	}
	
	public void setTimestamp(Integer timestamp) {
		this.timestamp = timestamp;
	}
	
	public int getMagic() {
		return magic;
	}
	
	public void setMagic(int magic) {
		this.magic = magic;
	}
	
	public int getGw_volume() {
		return gw_volume;
	}
	
	public void setGw_volume(int gw_volume) {
		this.gw_volume = gw_volume;
	}
	
	public int getGw_open_price() {
		return gw_open_price;
	}

	public void setGw_open_price(int gw_open_price) {
		this.gw_open_price = gw_open_price;
	}

	public int getGw_close_price() {
		return gw_close_price;
	}

	public void setGw_close_price(int gw_close_price) {
		this.gw_close_price = gw_close_price;
	}

	public Timestamp getModify_time() {
		return modify_time;
	}
	
	public void setModify_time(Timestamp modify_time) {
		this.modify_time = modify_time;
	}
	
	@Override
	public String toString() {
		return "Trade [ticket=" + ticket + ", login=" + login + ", symbol=" + symbol + ", digits=" + digits + ", cmd="
				+ cmd + ", volume=" + volume + ", open_time=" + open_time + ", open_price=" + open_price + ", sl=" + sl
				+ ", tp=" + tp + ", close_time=" + close_time + ", expiration=" + expiration + ", reason=" + reason
				+ ", conv_rate1=" + conv_rate1 + ", conv_rate2=" + conv_rate2 + ", commission=" + commission
				+ ", commission_agent=" + commission_agent + ", swaps=" + swaps + ", close_price=" + close_price
				+ ", profit=" + profit + ", taxes=" + taxes + ", comment=" + comment + ", internal_id=" + internal_id
				+ ", margin_rate=" + margin_rate + ", timestamp=" + timestamp + ", magic=" + magic + ", gw_volume="
				+ gw_volume + ", gw_openPrice=" + gw_open_price + ", gw_closePrice=" + gw_close_price + ", modify_time="
				+ modify_time + "]";
	}
	
}
