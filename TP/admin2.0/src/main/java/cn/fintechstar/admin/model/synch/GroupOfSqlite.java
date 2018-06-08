package cn.fintechstar.admin.model.synch;

public class GroupOfSqlite {
	private Integer id;
	private Integer groupId;
	private String groupName;
	private Integer securityId;
	private int securitySpread;
	private String timePeriod;
	private long maximumAmounts;
	private long minimumAmounts;
	private String groupPassword;
	private Double profit;
	private Integer adminId;
	private String modifyTime;
	private String description;
	
	public Integer getId() {
		return id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public Integer getGroupId() {
		return groupId;
	}
	
	public void setGroupId(Integer groupId) {
		this.groupId = groupId;
	}
	
	public String getGroupName() {
		return groupName;
	}
	
	public void setGroupName(String groupName) {
		this.groupName = groupName;
	}
	
	public Integer getSecurityId() {
		return securityId;
	}
	
	public void setSecurityId(Integer securityId) {
		this.securityId = securityId;
	}
	
	public int getSecuritySpread() {
		return securitySpread;
	}
	
	public void setSecuritySpread(int securitySpread) {
		this.securitySpread = securitySpread;
	}
	
	public String getTimePeriod() {
		return timePeriod;
	}
	
	public void setTimePeriod(String timePeriod) {
		this.timePeriod = timePeriod;
	}
	
	public long getMaximumAmounts() {
		return maximumAmounts;
	}
	
	public void setMaximumAmounts(long maximumAmounts) {
		this.maximumAmounts = maximumAmounts;
	}
	
	public long getMinimumAmounts() {
		return minimumAmounts;
	}
	
	public void setMinimumAmounts(long minimumAmounts) {
		this.minimumAmounts = minimumAmounts;
	}
	
	public String getGroupPassword() {
		return groupPassword;
	}
	
	public void setGroupPassword(String groupPassword) {
		this.groupPassword = groupPassword;
	}
	
	public Double getProfit() {
		return profit;
	}

	public void setProfit(Double profit) {
		this.profit = profit;
	}

	public Integer getAdminId() {
		return adminId;
	}
	
	public void setAdminId(Integer adminId) {
		this.adminId = adminId;
	}
	
	public String getModifyTime() {
		return modifyTime;
	}
	
	public void setModifyTime(String modifyTime) {
		this.modifyTime = modifyTime;
	}
	
	public String getDescription() {
		return description;
	}
	
	public void setDescription(String description) {
		this.description = description;
	}

}
