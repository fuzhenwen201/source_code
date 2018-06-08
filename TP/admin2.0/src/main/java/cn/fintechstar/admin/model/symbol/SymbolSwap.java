package cn.fintechstar.admin.model.symbol;

import com.google.gson.annotations.SerializedName;

public class SymbolSwap {

	@SerializedName("symbolid")
	private int symbolId;
	
	@SerializedName("isenableswap")
	private boolean isEnableSwap;

	private String type;
	
	@SerializedName("longposition")
	private double longPosition;

	@SerializedName("shortposition")
	private double shortPosition;
	
	@SerializedName("thirddayswap")
	private String thirdDaySwap;
	
	@SerializedName("isuseopenprice")
	private boolean isUseOpenPrice;
	
	@SerializedName("ischangevariation")
	private boolean isChangeVariation;

	
	public int getSymbolId() {
		return symbolId;
	}

	public void setSymbolId(int symbolId) {
		this.symbolId = symbolId;
	}

	public boolean isEnableSwap() {
		return isEnableSwap;
	}

	public void setEnableSwap(boolean isEnableSwap) {
		this.isEnableSwap = isEnableSwap;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
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

	public String getThirdDaySwap() {
		return thirdDaySwap;
	}

	public void setThirdDaySwap(String thirdDaySwap) {
		this.thirdDaySwap = thirdDaySwap;
	}

	public boolean isUseOpenPrice() {
		return isUseOpenPrice;
	}

	public void setUseOpenPrice(boolean isUseOpenPrice) {
		this.isUseOpenPrice = isUseOpenPrice;
	}

	public boolean isChangeVariation() {
		return isChangeVariation;
	}

	public void setChangeVariation(boolean isChangeVariation) {
		this.isChangeVariation = isChangeVariation;
	}

	@Override
	public String toString() {
		return "SymbolSwap [symbolId=" + symbolId + ", isEnableSwap=" + isEnableSwap + ", type=" + type
				+ ", longPosition=" + longPosition + ", shortPosition=" + shortPosition + ", thirdDaySwap="
				+ thirdDaySwap + ", isUseOpenPrice=" + isUseOpenPrice + ", isChangeVariation=" + isChangeVariation
				+ "]";
	}

}
