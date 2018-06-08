package com.broctagon.trad.rm.obj.message;

public class BoScaleMsg {
	private long openprice;
	private int nooftrade;
	private long totalvolume;

	public long getOpenprice() {
		return openprice;
	}
	public void setOpenprice(long openprice) {
		this.openprice = openprice;
	}
	public int getNoOftrade() {
		return nooftrade;
	}
	public void setNooftrade(int nooftrade) {
		this.nooftrade = nooftrade;
	}
	public long getTotalvolume() {
		return totalvolume;
	}
	public void setTotalvolume(long totalvolume) {
		this.totalvolume = totalvolume;
	}

}
