/**
 * 
 */
package com.broctagon.trad.rm.obj.message;

/**
 * @author imdadullah
 *
 */
public class SymbolBoMsg {
	private int assetid;
	private long totalupvolume;
	private long totaldownvolume;
	private long netvolume;
	
	public SymbolBoMsg() {
		this.assetid = 0;
		this.totalupvolume = 0;
		this.totaldownvolume = 0;
		this.netvolume = 0;
	}

	public int getAssetid() {
		return assetid;
	}

	public void setAssetid(int assetid) {
		this.assetid = assetid;
	}

	public long getTotalupvolume() {
		return totalupvolume;
	}

	public void setTotalupvolume(long totalupvolume) {
		this.totalupvolume = totalupvolume;
	}

	public long getTotaldownvolume() {
		return totaldownvolume;
	}

	public void setTotaldownvolume(long totaldownvolume) {
		this.totaldownvolume = totaldownvolume;
	}

	public long getNetvolume() {
		return netvolume;
	}

	public void setNetvolume(long netvolume) {
		this.netvolume = netvolume;
	}
	
	
}
