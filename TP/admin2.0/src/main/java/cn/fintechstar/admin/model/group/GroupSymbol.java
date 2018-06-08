package cn.fintechstar.admin.model.group;

import java.util.Date;

import com.google.gson.annotations.SerializedName;

public class GroupSymbol {
	
	@SerializedName("groupid")
	private int groupId;
	
	@SerializedName("symbolid")
	private Integer symbolId;
	
	@SerializedName("longpositionswap")
	private double longPositionSwap;
	
	@SerializedName("shortpositionswap")
	private double shortPositionSwap;
	
	@SerializedName("marginpercentage")
	private double marginPercentage;
	
	@SerializedName("modifiedtime")
	private Date modifiedTime;

	
	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public Integer getSymbolId() {
		return symbolId;
	}

	public void setSymbolId(Integer symbolId) {
		this.symbolId = symbolId;
	}

	public double getLongPositionSwap() {
		return longPositionSwap;
	}

	public void setLongPositionSwap(double longPositionSwap) {
		this.longPositionSwap = longPositionSwap;
	}

	public double getShortPositionSwap() {
		return shortPositionSwap;
	}

	public void setShortPositionSwap(double shortPositionSwap) {
		this.shortPositionSwap = shortPositionSwap;
	}

	public double getMarginPercentage() {
		return marginPercentage;
	}

	public void setMarginPercentage(double marginPercentage) {
		this.marginPercentage = marginPercentage;
	}

	public Date getModifiedTime() {
		return modifiedTime;
	}

	public void setModifiedTime(Date modifiedTime) {
		this.modifiedTime = modifiedTime;
	}

	@Override
	public String toString() {
		return "GroupSymbol [groupId=" + groupId + ", symbolId=" + symbolId + ", longPositionSwap=" + longPositionSwap
				+ ", shortPositionSwap=" + shortPositionSwap + ", marginPercentage=" + marginPercentage + "]";
	}
	
}
