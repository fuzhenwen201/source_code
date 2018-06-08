package cn.fintechstar.admin.model.group;

import com.google.gson.annotations.SerializedName;

public class TimePeriod {

	@SerializedName("groupid")
	private int groupId; // Here groupTCId and groupId are combined and work as composite key
	
	private int period;
	
	@SerializedName("profitpercentage")
	private int profitPercentage;

	
	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public int getPeriod() {
		return period;
	}

	public void setPeriod(int period) {
		this.period = period;
	}

	public int getProfitPercentage() {
		return profitPercentage;
	}

	public void setProfitPercentage(int profitPercentage) {
		this.profitPercentage = profitPercentage;
	}

	@Override
	public String toString() {
		return "TimePeriod [groupId=" + groupId + ", period=" + period + ", profitPercentage=" + profitPercentage + "]";
	}

}
