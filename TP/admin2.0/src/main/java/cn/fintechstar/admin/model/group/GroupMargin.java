package cn.fintechstar.admin.model.group;

import java.util.Date;

import com.google.gson.annotations.SerializedName;

public class GroupMargin {

	@SerializedName("groupid")
	private int groupId	;
	
	@SerializedName("calllevel")
	private int callLevel;
	
	@SerializedName("stopoutlevel")
	private String stopOutLevel;
	
	@SerializedName("levelformat")
	private String levelFormat;

	@SerializedName("freemargin")
	private String freeMargin;
	
	@SerializedName("virtualcredit")
	private double virtualCredit;
	
	@SerializedName("ishedgedmargincalculated")
	private Boolean isHedgedMarginCalulated;
	
	@SerializedName("isskiphedged")
	private Boolean isSkipHedged;
	
	@SerializedName("modifiedtime")
	private Date modifiedTime;

	
	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public int getCallLevel() {
		return callLevel;
	}

	public void setCallLevel(int callLevel) {
		this.callLevel = callLevel;
	}

	public String getStopOutLevel() {
		return stopOutLevel;
	}

	public void setStopOutLevel(String stopOutLevel) {
		this.stopOutLevel = stopOutLevel;
	}

	public String getLevelFormat() {
		return levelFormat;
	}

	public void setLevelFormat(String levelFormat) {
		this.levelFormat = levelFormat;
	}

	public String getFreeMargin() {
		return freeMargin;
	}

	public void setFreeMargin(String freeMargin) {
		this.freeMargin = freeMargin;
	}

	public double getVirtualCredit() {
		return virtualCredit;
	}

	public void setVirtualCredit(double virtualCredit) {
		this.virtualCredit = virtualCredit;
	}

	public Boolean getIsHedgedMarginCalulated() {
		return isHedgedMarginCalulated;
	}

	public void setIsHedgedMarginCalulated(Boolean isHedgedMarginCalulated) {
		this.isHedgedMarginCalulated = isHedgedMarginCalulated;
	}

	public Boolean getIsSkipHedged() {
		return isSkipHedged;
	}

	public void setIsSkipHedged(Boolean isSkipHedged) {
		this.isSkipHedged = isSkipHedged;
	}

	public Date getModifiedTime() {
		return modifiedTime;
	}

	public void setModifiedTime(Date modifiedTime) {
		this.modifiedTime = modifiedTime;
	}

	@Override
	public String toString() {
		return "GroupMargin [groupId=" + groupId + ", callLevel=" + callLevel + ", stopOutLevel=" + stopOutLevel
				+ ", levelFormat=" + levelFormat + ", freeMargin=" + freeMargin + ", virtualCredit=" + virtualCredit
				+ ", isHedgedMarginCalulated=" + isHedgedMarginCalulated + ", isSkipHedged=" + isSkipHedged
				+ "]";
	}
	
}
