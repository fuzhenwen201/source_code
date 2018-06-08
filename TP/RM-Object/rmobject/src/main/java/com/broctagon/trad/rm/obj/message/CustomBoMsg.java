package com.broctagon.trad.rm.obj.message;

import java.util.List;

/**
 * 
 * @author imdadullah
 *
 */

public class CustomBoMsg {

	private int assetid;
	private long totalvolume;
	private long averagevolume;
	private long lowestprice;
	private long averageprice;
	private int nooforders;
	private int direction;
	
	public int getDirection() {
		return direction;
	}
	public void setDirection(int direction) {
		this.direction = direction;
	}
	public List<BoScaleMsg> getScale() {
		return scale;
	}
	public void setScale(List<BoScaleMsg> scale) {
		this.scale = scale;
	}
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
	public int getNooforders() {
		return nooforders;
	}
	public void setNooforders(int nooforders) {
		this.nooforders = nooforders;
	}

	
}
