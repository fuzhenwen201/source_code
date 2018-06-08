package com.broctagon.trad.reportserver.model;

import java.sql.Timestamp;

public class Price {

	private String symbol;
	private Timestamp time;
	private double bid;
	private double ask;
	private double low;
	private double high;
	private int direction;
	private int digits;
	private int spread;
	private Timestamp modify_time;
	
	
	public String getSymbol() {
		return symbol;
	}
	
	public void setSymbol(String symbol) {
		this.symbol = symbol;
	}
	
	public Timestamp getTime() {
		return time;
	}
	
	public void setTime(Timestamp time) {
		this.time = time;
	}
	
	public double getBid() {
		return bid;
	}
	
	public void setBid(double bid) {
		this.bid = bid;
	}
	
	public double getAsk() {
		return ask;
	}
	
	public void setAsk(double ask) {
		this.ask = ask;
	}
	
	public double getLow() {
		return low;
	}
	
	public void setLow(double low) {
		this.low = low;
	}
	
	public double getHigh() {
		return high;
	}
	
	public void setHigh(double high) {
		this.high = high;
	}
	
	public int getDirection() {
		return direction;
	}
	
	public void setDirection(int direction) {
		this.direction = direction;
	}
	
	public int getDigits() {
		return digits;
	}
	
	public void setDigits(int digits) {
		this.digits = digits;
	}
	
	public int getSpread() {
		return spread;
	}
	
	public void setSpread(int spread) {
		this.spread = spread;
	}
	
	public Timestamp getModify_time() {
		return modify_time;
	}

	public void setModify_time(Timestamp modify_time) {
		this.modify_time = modify_time;
	}

	@Override
	public String toString() {
		return "Price [symbol=" + symbol + ", time=" + time + ", bid=" + bid + ", ask=" + ask + ", low=" + low
				+ ", high=" + high + ", direction=" + direction + ", digits=" + digits + ", spread=" + spread
				+ ", modifyTime=" + modify_time + "]";
	}
	
}
