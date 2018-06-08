package com.broctagon.trad.rm.db.model;

import java.util.List;

public class BoModel {
	private int assetId;
	private int totalVolume;
	private int noOfOrders;
	private int averageVolume;
	private int lowestPrice;
	private int averagePrice;
	private List<BoScaleModel> scall;
	
	public int getAssetId() {
		return assetId;
	}
	public void setAssetId(int assetId) {
		this.assetId = assetId;
	}
	public int getTotalVolume() {
		return totalVolume;
	}
	public void setTotalVolume(int totalVolume) {
		this.totalVolume = totalVolume;
	}
	public int getNoOfOrders() {
		return noOfOrders;
	}
	public void setNoOfOrders(int noOfOrders) {
		this.noOfOrders = noOfOrders;
	}
	public int getAverageVolume() {
		return averageVolume;
	}
	public void setAverageVolume(int averageVolume) {
		this.averageVolume = averageVolume;
	}
	public int getLowestPrice() {
		return lowestPrice;
	}
	public void setLowestPrice(int lowestPrice) {
		this.lowestPrice = lowestPrice;
	}
	public int getAveragePrice() {
		return averagePrice;
	}
	public void setAveragePrice(int averagePrice) {
		this.averagePrice = averagePrice;
	}
	public List<BoScaleModel> getScall() {
		return scall;
	}
	public void setScall(List<BoScaleModel> scall) {
		this.scall = scall;
	}
	
}
