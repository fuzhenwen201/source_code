package com.broctagon.trad.rm.db.model;

public class BoScaleModel {
	private int openPrice;
	private int noOfTrade;
	private int totalVolume;
	
	public int getOpenPrice() {
		return openPrice;
	}
	public void setOpenPrice(int openPrice) {
		this.openPrice = openPrice;
	}
	public int getNoOfTrade() {
		return noOfTrade;
	}
	public void setNoOfTrade(int noOfTrade) {
		this.noOfTrade = noOfTrade;
	}
	public int getTotalVolume() {
		return totalVolume;
	}
	public void setTotalVolume(int totalVolume) {
		this.totalVolume = totalVolume;
	}
}
