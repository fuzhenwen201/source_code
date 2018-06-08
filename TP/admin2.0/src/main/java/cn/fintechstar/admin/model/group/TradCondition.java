package cn.fintechstar.admin.model.group;

import java.util.Date;
import java.util.List;

import com.google.gson.annotations.SerializedName;

public class TradCondition {

	@SerializedName("groupid")
	private int groupId	;
	
	@SerializedName("minimumboamount")
	private long minimumBoAmount;
	
	@SerializedName("maximumboamount")
	private long maximumBoAmount;
	
	@SerializedName("timeperiod")
	private List<TimePeriod> timePeriods;
	
	@SerializedName("modifiedtime")
	private Date modifiedTime;
	
	private double payout;

	
	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public long getMinimumBoAmount() {
		return minimumBoAmount;
	}

	public void setMinimumBoAmount(long minimumBoAmount) {
		this.minimumBoAmount = minimumBoAmount;
	}

	public long getMaximumBoAmount() {
		return maximumBoAmount;
	}

	public void setMaximumBoAmount(long maximumBoAmount) {
		this.maximumBoAmount = maximumBoAmount;
	}

	public List<TimePeriod> getTimePeriods() {
		return timePeriods;
	}

	public void setTimePeriods(List<TimePeriod> timePeriods) {
		this.timePeriods = timePeriods;
	}

	public Date getModifiedTime() {
		return modifiedTime;
	}

	public void setModifiedTime(Date modifiedTime) {
		this.modifiedTime = modifiedTime;
	}

	public double getPayout() {
		return payout;
	}

	public void setPayout(double payout) {
		this.payout = payout;
	}

	@Override
	public String toString() {
		return "TradCondition [groupId=" + groupId + ", minimumBoAmount=" + minimumBoAmount + ", maximumBoAmount="
				+ maximumBoAmount + ", timePeriods=" + timePeriods + ", payout=" + payout + "]";
	}
	
}
