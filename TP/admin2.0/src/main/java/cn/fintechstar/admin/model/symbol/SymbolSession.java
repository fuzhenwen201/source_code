package cn.fintechstar.admin.model.symbol;

import java.util.Date;
import java.sql.Time;

public class SymbolSession {

	private int symbolId;
	private String day;
	private Time openQuote;
	private Time closeQuote;
	private Time openTrade;
	private Time closeTrade;
	private Date modifiedTime;

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

	public Time getOpenQuote() {
		return openQuote;
	}

	public void setOpenQuote(Time openQuote) {
		this.openQuote = openQuote;
	}

	public Time getCloseQuote() {
		return closeQuote;
	}

	public void setCloseQuote(Time closeQuote) {
		this.closeQuote = closeQuote;
	}

	public Time getOpenTrade() {
		return openTrade;
	}

	public void setOpenTrade(Time openTrade) {
		this.openTrade = openTrade;
	}

	public Time getCloseTrade() {
		return closeTrade;
	}

	public void setCloseTrade(Time closeTrade) {
		this.closeTrade = closeTrade;
	}

	public Date getModifiedTime() {
		return modifiedTime;
	}

	public void setModifiedTime(Date modifiedTime) {
		this.modifiedTime = modifiedTime;
	}

	@Override
	public String toString() {
		return "SymbolSession [symbolId=" + symbolId + ", day=" + day + ", openQuote=" + openQuote + ", closeQuote="
				+ closeQuote + ", openTrade=" + openTrade + ", closeTrade=" + closeTrade + ", modifiedTime="
				+ modifiedTime + "]";
	}

}
