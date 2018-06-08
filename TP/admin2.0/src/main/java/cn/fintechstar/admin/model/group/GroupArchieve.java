package cn.fintechstar.admin.model.group;

import java.util.Date;

import com.google.gson.annotations.SerializedName;

public class GroupArchieve {

	@SerializedName("groupid")
	private int groupId;
	
	@SerializedName("inactiveperiod")
	private int inActivePeriod;
	
	@SerializedName("maxbalance")
	private double maxBalance;
	
	@SerializedName("pendingdeletedorder")
	private int pendingDeletedOrder;
	
	@SerializedName("modifiedtime")
	private Date modifiedTime;

	
	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public int getInActivePeriod() {
		return inActivePeriod;
	}

	public void setInActivePeriod(int inActivePeriod) {
		this.inActivePeriod = inActivePeriod;
	}

	public double getMaxBalance() {
		return maxBalance;
	}

	public void setMaxBalance(double maxBalance) {
		this.maxBalance = maxBalance;
	}

	public int getPendingDeletedOrder() {
		return pendingDeletedOrder;
	}

	public void setPendingDeletedOrder(int pendingDeletedOrder) {
		this.pendingDeletedOrder = pendingDeletedOrder;
	}

	public Date getModifiedTime() {
		return modifiedTime;
	}

	public void setModifiedTime(Date modifiedTime) {
		this.modifiedTime = modifiedTime;
	}

	@Override
	public String toString() {
		return "GroupArchieve [groupId=" + groupId + ", inActivePeriod=" + inActivePeriod + ", maxBalance=" + maxBalance
				+ ", pendingDeletedOrder=" + pendingDeletedOrder + "]";
	}

}
