package cn.fintechstar.admin.model.group;

import java.util.List;

public class GroupDetailsVo {

	private int groupId;
	private Boolean isEnable;
	private String groupName;
	private String ownerName;
	private int callLevel;
	private String stopOutLevel;
	private List<String> securityList;

	
	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public Boolean getIsEnable() {
		return isEnable;
	}

	public void setIsEnable(Boolean isEnable) {
		this.isEnable = isEnable;
	}

	public String getGroupName() {
		return groupName;
	}

	public void setGroupName(String groupName) {
		this.groupName = groupName;
	}

	public String getOwnerName() {
		return ownerName;
	}

	public void setOwnerName(String ownerName) {
		this.ownerName = ownerName;
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

	public List<String> getSecurityList() {
		return securityList;
	}

	public void setSecurityList(List<String> securityList) {
		this.securityList = securityList;
	}

	@Override
	public String toString() {
		return "GroupDetailsVo [groupId=" + groupId + ", isEnable=" + isEnable + ", groupName=" + groupName
				+ ", ownerName=" + ownerName + ", callLevel=" + callLevel + ", stopOutLevel=" + stopOutLevel
				+ ", securityList=" + securityList + "]";
	}

}
