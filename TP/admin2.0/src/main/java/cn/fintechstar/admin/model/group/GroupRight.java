package cn.fintechstar.admin.model.group;

import java.util.Date;

import com.google.gson.annotations.SerializedName;

public class GroupRight {
	
	@SerializedName("groupid")
	private int groupId	;
	
	private int timeout;
	
	@SerializedName("newsview")
	private String newsView;

	@SerializedName("newslanguage")
	private String newsLanguage;
	
	@SerializedName("maxsymbol")
	private int maxSymbol;
	
	@SerializedName("maxorder")
	private int maxOrder;
	
	@SerializedName("tradesignals")
	private String tradeSignals;
	
	@SerializedName("ismailsystem")
	private int isMailSystem;
	
	@SerializedName("isswap")
	private int isSwap;
	
	@SerializedName("isea")
	private int isEA;
	
	@SerializedName("ispendingorderexpiry")
	private int isPendingOrderExpiry;
	
	@SerializedName("istrailingstop")
	private int isTrailingStop;
	
	@SerializedName("isreqpriceinie")
	private int isReqPriceInIE;
	
	@SerializedName("ishedgeprohibit")
	private int isHedgeProhibit;
	
	@SerializedName("isfifo")
	private int isFIFO;
	
	@SerializedName("isparclose")
	private int isParClose;
	
	@SerializedName("isriskwarning")
	private int isRiskWarning;
	
	@SerializedName("modifiedtime")
	private Date modifiedTime;

	
	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public int getTimeout() {
		return timeout;
	}

	public void setTimeout(int timeout) {
		this.timeout = timeout;
	}

	public String getNewsView() {
		return newsView;
	}

	public void setNewsView(String newsView) {
		this.newsView = newsView;
	}

	public String getNewsLanguage() {
		return newsLanguage;
	}

	public void setNewsLanguage(String newsLanguage) {
		this.newsLanguage = newsLanguage;
	}

	public int getMaxSymbol() {
		return maxSymbol;
	}

	public void setMaxSymbol(int maxSymbol) {
		this.maxSymbol = maxSymbol;
	}

	public int getMaxOrder() {
		return maxOrder;
	}

	public void setMaxOrder(int maxOrder) {
		this.maxOrder = maxOrder;
	}

	public String getTradeSignals() {
		return tradeSignals;
	}

	public void setTradeSignals(String tradeSignals) {
		this.tradeSignals = tradeSignals;
	}

	public int getIsMailSystem() {
		return isMailSystem;
	}

	public void setIsMailSystem(int isMailSystem) {
		this.isMailSystem = isMailSystem;
	}

	public int getIsSwap() {
		return isSwap;
	}

	public void setIsSwap(int isSwap) {
		this.isSwap = isSwap;
	}

	public int getIsEA() {
		return isEA;
	}

	public void setIsEA(int isEA) {
		this.isEA = isEA;
	}

	public int getIsPendingOrderExpiry() {
		return isPendingOrderExpiry;
	}

	public void setIsPendingOrderExpiry(int isPendingOrderExpiry) {
		this.isPendingOrderExpiry = isPendingOrderExpiry;
	}

	public int getIsTrailingStop() {
		return isTrailingStop;
	}

	public void setIsTrailingStop(int isTrailingStop) {
		this.isTrailingStop = isTrailingStop;
	}

	public int getIsReqPriceInIE() {
		return isReqPriceInIE;
	}

	public void setIsReqPriceInIE(int isReqPriceInIE) {
		this.isReqPriceInIE = isReqPriceInIE;
	}

	public int getIsHedgeProhibit() {
		return isHedgeProhibit;
	}

	public void setIsHedgeProhibit(int isHedgeProhibit) {
		this.isHedgeProhibit = isHedgeProhibit;
	}

	public int getIsFIFO() {
		return isFIFO;
	}

	public void setIsFIFO(int isFIFO) {
		this.isFIFO = isFIFO;
	}

	public int getIsParClose() {
		return isParClose;
	}

	public void setIsParClose(int isParClose) {
		this.isParClose = isParClose;
	}

	public int getIsRiskWarning() {
		return isRiskWarning;
	}

	public void setIsRiskWarning(int isRiskWarning) {
		this.isRiskWarning = isRiskWarning;
	}

	public Date getModifiedTime() {
		return modifiedTime;
	}

	public void setModifiedTime(Date modifiedTime) {
		this.modifiedTime = modifiedTime;
	}

	@Override
	public String toString() {
		return "GroupRight [groupId=" + groupId + ", timeout=" + timeout + ", newsView=" + newsView + ", newsLanguage="
				+ newsLanguage + ", maxSymbol=" + maxSymbol + ", maxOrder=" + maxOrder + ", tradeSignals="
				+ tradeSignals + ", isMailSystem=" + isMailSystem + ", isSwap=" + isSwap + ", isEA=" + isEA
				+ ", isPendingOrderExpiry=" + isPendingOrderExpiry + ", isTrailingStop=" + isTrailingStop
				+ ", isReqPriceInIE=" + isReqPriceInIE + ", isHedgeProhibit=" + isHedgeProhibit + ", isFIFO=" + isFIFO
				+ ", isParClose=" + isParClose + ", isRiskWarning=" + isRiskWarning + "]";
	}
	
}
