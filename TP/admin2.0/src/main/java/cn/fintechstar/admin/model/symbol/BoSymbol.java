package cn.fintechstar.admin.model.symbol;

public class BoSymbol {

	private int symbolId;
	private int boCalType;
	private double returnRate;

	
	public int getSymbolId() {
		return symbolId;
	}

	public void setSymbolId(int symbolId) {
		this.symbolId = symbolId;
	}

	public int getBoCalType() {
		return boCalType;
	}

	public void setBoCalType(int boCalType) {
		this.boCalType = boCalType;
	}

	public double getReturnRate() {
		return returnRate;
	}

	public void setReturnRate(double returnRate) {
		this.returnRate = returnRate;
	}

	@Override
	public String toString() {
		return "BoSymbol [symbolId=" + symbolId + ", boCalType=" + boCalType + ", returnRate=" + returnRate + "]";
	}

}
