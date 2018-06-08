package com.broctagon.trad.rm.obj.message;

import java.util.List;


public class TopFiveBoMsg {
	
	private int assetid;
	private long totalvolume;
	private int nooforders;
	private long averagevolume;
	private long lowestprice;
	private long averageprice;
	private List<BoScaleMsg> scale;
	public int getAssetid() {
		return assetid;
	}
	public void setAssetid(int assetid) {
		this.assetid = assetid;
	}
	public long getTotalvolume() {
		return totalvolume;
	}
	public void setTotalvolume(long totalvolume) {
		this.totalvolume = totalvolume;
	}
	public int getNooforders() {
		return nooforders;
	}
	public void setNooforders(int nooforders) {
		this.nooforders = nooforders;
	}
	public long getAveragevolume() {
		return averagevolume;
	}
	public void setAveragevolume(long averagevolume) {
		this.averagevolume = averagevolume;
	}
	public long getLowestprice() {
		return lowestprice;
	}
	public void setLowestprice(long lowestprice) {
		this.lowestprice = lowestprice;
	}
	public long getAverageprice() {
		return averageprice;
	}
	public void setAverageprice(long averageprice) {
		this.averageprice = averageprice;
	}
	public List<BoScaleMsg> getScale() {
		return scale;
	}
	public void setScale(List<BoScaleMsg> scale) {
		this.scale = scale;
	}
	/**
	 * 
	 */
	
	/**
	 * @param assetid
	 * @param totalvolume
	 * @param nooforders
	 * @param averagevolume
	 * @param lowestprice
	 * @param averageprice
	 * @param scale
	 */
	public TopFiveBoMsg() {
		this.assetid = 0;
		this.totalvolume = 0;
		this.nooforders = 0;
		this.averagevolume = 0;
		this.lowestprice = 0;
		this.averageprice = 0;
		this.scale = null;
	}
	
	
}
