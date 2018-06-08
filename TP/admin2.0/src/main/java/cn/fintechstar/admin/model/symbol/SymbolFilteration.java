package cn.fintechstar.admin.model.symbol;

import com.google.gson.annotations.SerializedName;

public class SymbolFilteration {

	@SerializedName("symbolid")
	private int symbolId;
	
	@SerializedName("isrealtimedatafeed")
	private boolean isRealtimeDataFeed;
	
	@SerializedName("issaveallincomingprice")
	private boolean isSaveAllIncomingPrice;
	
	@SerializedName("filtrationlevel")
	private int filtrationLevel;
	
	@SerializedName("automaticlimit")
	private double automaticLimit;
	
	private int filter;
	
	@SerializedName("ignoreQuot")
	private int ignoreQuot;
	
	private int smoothing;

	
	public int getSymbolId() {
		return symbolId;
	}

	public void setSymbolId(int symbolId) {
		this.symbolId = symbolId;
	}

	public boolean isRealtimeDataFeed() {
		return isRealtimeDataFeed;
	}

	public void setRealtimeDataFeed(boolean isRealtimeDataFeed) {
		this.isRealtimeDataFeed = isRealtimeDataFeed;
	}

	public boolean isSaveAllIncomingPrice() {
		return isSaveAllIncomingPrice;
	}

	public void setSaveAllIncomingPrice(boolean isSaveAllIncomingPrice) {
		this.isSaveAllIncomingPrice = isSaveAllIncomingPrice;
	}

	public int getFiltrationLevel() {
		return filtrationLevel;
	}

	public void setFiltrationLevel(int filtrationLevel) {
		this.filtrationLevel = filtrationLevel;
	}

	public double getAutomaticLimit() {
		return automaticLimit;
	}

	public void setAutomaticLimit(double automaticLimit) {
		this.automaticLimit = automaticLimit;
	}

	public int getFilter() {
		return filter;
	}

	public void setFilter(int filter) {
		this.filter = filter;
	}

	public int getIgnoreQuot() {
		return ignoreQuot;
	}

	public void setIgnoreQuot(int ignoreQuot) {
		this.ignoreQuot = ignoreQuot;
	}

	public int getSmoothing() {
		return smoothing;
	}

	public void setSmoothing(int smoothing) {
		this.smoothing = smoothing;
	}

	@Override
	public String toString() {
		return "SymbolFilteration [symbolId=" + symbolId + ", isRealtimeDataFeed=" + isRealtimeDataFeed
				+ ", isSaveAllIncomingPrice=" + isSaveAllIncomingPrice + ", filtrationLevel=" + filtrationLevel
				+ ", automaticLimit=" + automaticLimit + ", filter=" + filter + ", ignoreQuot=" + ignoreQuot
				+ ", smoothing=" + smoothing + "]";
	}
	
}
