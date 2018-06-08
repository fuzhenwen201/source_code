package cn.fintechstar.admin.model.symbol;

import com.google.gson.annotations.SerializedName;

public class BoSymbolVo {

	@SerializedName("caltype")
	private int calType;
	
	@SerializedName("returnrate")
	private double returnRate;

	
	public int getCalType() {
		return calType;
	}

	public void setCalType(int calType) {
		this.calType = calType;
	}

	public double getReturnRate() {
		return returnRate;
	}

	public void setReturnRate(double returnRate) {
		this.returnRate = returnRate;
	}

}
