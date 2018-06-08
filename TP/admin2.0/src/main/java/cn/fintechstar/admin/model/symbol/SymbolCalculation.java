package cn.fintechstar.admin.model.symbol;

import com.google.gson.annotations.SerializedName;

public class SymbolCalculation {

	@SerializedName("symbolid")
	private int symbolId;
	
	@SerializedName("contractsize")
	private float contractSize;
	
	@SerializedName("initialmargin")
	private double initialMargin;
	
	private float maintenace;
	
	private float hedged;
	
	@SerializedName("tickSize")
	private float tickSize;
	
	@SerializedName("tickprice")
	private double tickPrice;
	
	@SerializedName("marginpercentage")
	private double marginPercentage;

	@SerializedName("margincal")
	private String marginCal;
	
	@SerializedName("profitcal")
	private String profitCal;
	
	@SerializedName("isshmmode")
	private boolean isShmMode;
	
	
	public int getSymbolId() {
		return symbolId;
	}

	public void setSymbolId(int symbolId) {
		this.symbolId = symbolId;
	}

	public float getContractSize() {
		return contractSize;
	}

	public void setContractSize(float contractSize) {
		this.contractSize = contractSize;
	}

	public double getInitialMargin() {
		return initialMargin;
	}

	public void setInitialMargin(double initialMargin) {
		this.initialMargin = initialMargin;
	}

	public float getMaintenace() {
		return maintenace;
	}

	public void setMaintenace(float maintenace) {
		this.maintenace = maintenace;
	}

	public float getHedged() {
		return hedged;
	}

	public void setHedged(float hedged) {
		this.hedged = hedged;
	}

	public float getTickSize() {
		return tickSize;
	}

	public void setTickSize(float tickSize) {
		this.tickSize = tickSize;
	}

	public double getTickPrice() {
		return tickPrice;
	}

	public void setTickPrice(double tickPrice) {
		this.tickPrice = tickPrice;
	}

	public double getMarginPercentage() {
		return marginPercentage;
	}

	public void setMarginPercentage(double marginPercentage) {
		this.marginPercentage = marginPercentage;
	}

	public String getMarginCal() {
		return marginCal;
	}

	public void setMarginCal(String marginCal) {
		this.marginCal = marginCal;
	}

	public String getProfitCal() {
		return profitCal;
	}

	public void setProfitCal(String profitCal) {
		this.profitCal = profitCal;
	}

	public boolean isShmMode() {
		return isShmMode;
	}

	public void setShmMode(boolean isShmMode) {
		this.isShmMode = isShmMode;
	}

	@Override
	public String toString() {
		return "SymbolCalculation [symbolId=" + symbolId + ", contractSize=" + contractSize + ", initialMargin="
				+ initialMargin + ", maintenace=" + maintenace + ", hedged=" + hedged + ", tickSize=" + tickSize
				+ ", tickPrice=" + tickPrice + ", marginPercentage=" + marginPercentage + ", marginCal=" + marginCal
				+ ", profitCal=" + profitCal + ", isShmMode=" + isShmMode + "]";
	}
	
}
