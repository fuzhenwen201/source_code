package cn.fintechstar.admin.model.synch;

public class SecurityOfGroupBO {

	private int groupId;
	private String groupName;
	private int securityId;
	private int spread;
	private int maximumBoAmount;
	private int minimumBoAmount;
	private double payout;
	private String modifyTime;

	
	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public String getGroupName() {
		return groupName;
	}

	public void setGroupName(String groupName) {
		this.groupName = groupName;
	}

	public int getSecurityId() {
		return securityId;
	}

	public void setSecurityId(int securityId) {
		this.securityId = securityId;
	}

	public int getSpread() {
		return spread;
	}

	public void setSpread(int spread) {
		this.spread = spread;
	}

	public int getMaximumBoAmount() {
		return maximumBoAmount;
	}

	public void setMaximumBoAmount(int maximumBoAmount) {
		this.maximumBoAmount = maximumBoAmount;
	}

	public int getMinimumBoAmount() {
		return minimumBoAmount;
	}

	public void setMinimumBoAmount(int minimumBoAmount) {
		this.minimumBoAmount = minimumBoAmount;
	}

	public double getPayout() {
		return payout;
	}

	public void setPayout(double payout) {
		this.payout = payout;
	}

	public String getModifyTime() {
		return modifyTime;
	}

	public void setModifyTime(String modifyTime) {
		this.modifyTime = modifyTime;
	}
	
	
}
