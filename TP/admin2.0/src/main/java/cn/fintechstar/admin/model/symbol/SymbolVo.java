package cn.fintechstar.admin.model.symbol;

import com.google.gson.annotations.SerializedName;

public class SymbolVo {

	@SerializedName("symbolid")
	private int symbolId;
	
	@SerializedName("symbolname")
	private String symbolName;
	
	@SerializedName("securityname")
	private String securityName;
	
	@SerializedName("executionmode")
	private String executionMode;
	
	private int filter;
	
	private String spread;
	
	private int stops;
	
	@SerializedName("longposition")
	private double longPosition;
	
	@SerializedName("shortposition")
	private double shortPosition;
	
	private int digits;
	
	private String trade;

	
	public int getSymbolId() {
		return symbolId;
	}

	public void setSymbolId(int symbolId) {
		this.symbolId = symbolId;
	}

	public String getSymbolName() {
		return symbolName;
	}

	public void setSymbolName(String symbolName) {
		this.symbolName = symbolName;
	}

	public String getSecurityName() {
		return securityName;
	}

	public void setSecurityName(String securityName) {
		this.securityName = securityName;
	}

	public String getExecutionMode() {
		return executionMode;
	}

	public void setExecutionMode(String executionMode) {
		this.executionMode = executionMode;
	}

	public int getFilter() {
		return filter;
	}

	public void setFilter(int filter) {
		this.filter = filter;
	}

	public String getSpread() {
		return spread;
	}

	public void setSpread(String spread) {
		this.spread = spread;
	}

	public int getStops() {
		return stops;
	}

	public void setStops(int stops) {
		this.stops = stops;
	}

	public double getLongPosition() {
		return longPosition;
	}

	public void setLongPosition(double longPosition) {
		this.longPosition = longPosition;
	}

	public double getShortPosition() {
		return shortPosition;
	}

	public void setShortPosition(double shortPosition) {
		this.shortPosition = shortPosition;
	}

	public int getDigits() {
		return digits;
	}

	public void setDigits(int digits) {
		this.digits = digits;
	}

	public String getTrade() {
		return trade;
	}

	public void setTrade(String trade) {
		this.trade = trade;
	}

}
