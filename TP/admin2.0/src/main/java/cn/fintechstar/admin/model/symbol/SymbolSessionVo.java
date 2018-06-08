package cn.fintechstar.admin.model.symbol;

import com.google.gson.annotations.SerializedName;

public class SymbolSessionVo {

	@SerializedName("symbolid")
	private int symbolId;
	
	private String day;
	
	@SerializedName("openquote")
	private String openQuote;
	
	@SerializedName("closequote")
	private String closeQuote;
	
	@SerializedName("opentrade")
	private String openTrade;
	
	@SerializedName("closetrade")
	private String closeTrade;

	
	public int getSymbolId() {
		return symbolId;
	}

	public void setSymbolId(int symbolId) {
		this.symbolId = symbolId;
	}

	public String getDay() {
		return day;
	}

	public void setDay(String day) {
		this.day = day;
	}

	public String getOpenQuote() {
		return openQuote;
	}

	public void setOpenQuote(String openQuote) {
		this.openQuote = openQuote;
	}

	public String getCloseQuote() {
		return closeQuote;
	}

	public void setCloseQuote(String closeQuote) {
		this.closeQuote = closeQuote;
	}

	public String getOpenTrade() {
		return openTrade;
	}

	public void setOpenTrade(String openTrade) {
		this.openTrade = openTrade;
	}

	public String getCloseTrade() {
		return closeTrade;
	}

	public void setCloseTrade(String closeTrade) {
		this.closeTrade = closeTrade;
	}
	
}
