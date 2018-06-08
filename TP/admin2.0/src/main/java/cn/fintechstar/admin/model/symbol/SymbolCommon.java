package cn.fintechstar.admin.model.symbol;

import com.google.gson.annotations.SerializedName;

public class SymbolCommon {

	@SerializedName("symbolid")
	private int symbolId;
	
	private String symbol;
	
	private String source;
	
	@SerializedName("symbolsourceid")
	private Integer symbolSourceId;
	
	@SerializedName("securityid")
	private Integer securityId;
	
	private int digit;
	
	private String description;
	
	private String security;
	
	@SerializedName("executionmode")
	private String executionMode;
	
	private String currency;
	
	private String trade;
	
	@SerializedName("backgroundcolor")
	private String backgroundColor;
	
	@SerializedName("margincurrency")
	private String marginCurrency;
	
	private int maxLot;
	
	private String orders;
	
	@SerializedName("defaultspread")
	private String defaultSpread;
	
	@SerializedName("islong")
	private boolean isLong;
	
	@SerializedName("limitstoplevel")
	private int limitStopLevel;
	
	@SerializedName("spreadbalbid")
	private int spreadBalBid;
	
	@SerializedName("spreadbalask")
	private int spreadBalAsk;
	
	@SerializedName("freezelevel")
	private int freezeLevel;

	
	public int getSymbolId() {
		return symbolId;
	}

	public void setSymbolId(int symbolId) {
		this.symbolId = symbolId;
	}

	public String getSymbol() {
		return symbol;
	}

	public void setSymbol(String symbol) {
		this.symbol = symbol;
	}

	public String getSource() {
		return source;
	}

	public void setSource(String source) {
		this.source = source;
	}

	public Integer getSymbolSourceId() {
		return symbolSourceId;
	}

	public void setSymbolSourceId(Integer symbolSourceId) {
		this.symbolSourceId = symbolSourceId;
	}

	public Integer getSecurityId() {
		return securityId;
	}

	public void setSecurityId(Integer securityId) {
		this.securityId = securityId;
	}

	public int getDigit() {
		return digit;
	}

	public void setDigit(int digit) {
		this.digit = digit;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getSecurity() {
		return security;
	}

	public void setSecurity(String security) {
		this.security = security;
	}

	public String getExecutionMode() {
		return executionMode;
	}

	public void setExecutionMode(String executionMode) {
		this.executionMode = executionMode;
	}

	public String getCurrency() {
		return currency;
	}

	public void setCurrency(String currency) {
		this.currency = currency;
	}

	public String getTrade() {
		return trade;
	}

	public void setTrade(String trade) {
		this.trade = trade;
	}

	public String getBackgroundColor() {
		return backgroundColor;
	}

	public void setBackgroundColor(String backgroundColor) {
		this.backgroundColor = backgroundColor;
	}

	public String getMarginCurrency() {
		return marginCurrency;
	}

	public void setMarginCurrency(String marginCurrency) {
		this.marginCurrency = marginCurrency;
	}

	public int getMaxLot() {
		return maxLot;
	}

	public void setMaxLot(int maxLot) {
		this.maxLot = maxLot;
	}

	public String getOrders() {
		return orders;
	}

	public void setOrders(String orders) {
		this.orders = orders;
	}

	public String getDefaultSpread() {
		return defaultSpread;
	}

	public void setDefaultSpread(String defaultSpread) {
		this.defaultSpread = defaultSpread;
	}

	public boolean isLong() {
		return isLong;
	}

	public void setLong(boolean isLong) {
		this.isLong = isLong;
	}

	public int getLimitStopLevel() {
		return limitStopLevel;
	}

	public void setLimitStopLevel(int limitStopLevel) {
		this.limitStopLevel = limitStopLevel;
	}

	public int getSpreadBalBid() {
		return spreadBalBid;
	}

	public void setSpreadBalBid(int spreadBalBid) {
		this.spreadBalBid = spreadBalBid;
	}

	public int getSpreadBalAsk() {
		return spreadBalAsk;
	}

	public void setSpreadBalAsk(int spreadBalAsk) {
		this.spreadBalAsk = spreadBalAsk;
	}

	public int getFreezeLevel() {
		return freezeLevel;
	}

	public void setFreezeLevel(int freezeLevel) {
		this.freezeLevel = freezeLevel;
	}

	@Override
	public String toString() {
		return "SymbolCommon [symbolId=" + symbolId + ", symbol=" + symbol + ", source=" + source + ", symbolSourceId="
				+ symbolSourceId + ", securityId=" + securityId + ", digit=" + digit + ", description=" + description
				+ ", security=" + security + ", executionMode=" + executionMode + ", currency=" + currency + ", trade="
				+ trade + ", backgroundColor=" + backgroundColor + ", marginCurrency=" + marginCurrency + ", maxLot="
				+ maxLot + ", orders=" + orders + ", defaultSpread=" + defaultSpread + ", isLong=" + isLong
				+ ", limitStopLevel=" + limitStopLevel + ", spreadBalBid=" + spreadBalBid + ", spreadBalAsk="
				+ spreadBalAsk + ", freezeLevel=" + freezeLevel + "]";
	}
	
}

