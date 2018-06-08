package cn.fintechstar.admin.model.security;

import java.util.Date;

public class Security {
	
	private int securityId;
	private String securityName;
	private int isEnable;   // 1 : enable ;  0 : disable
	private int isDeleted;  // 0 : in use ;  1 : deleted
	private String description;
	private String type;
	private Date createdTime; 
	private Date modifiedTime;

	public int getSecurityId() {
		return securityId;
	}

	public void setSecurityId(int securityId) {
		this.securityId = securityId;
	}

	public String getSecurityName() {
		return securityName;
	}

	public void setSecurityName(String securityName) {
		this.securityName = securityName;
	}

	public int getIsEnable() {
		return isEnable;
	}

	public void setIsEnable(int isEnable) {
		this.isEnable = isEnable;
	}

	public int getIsDeleted() {
		return isDeleted;
	}

	public void setIsDeleted(int isDeleted) {
		this.isDeleted = isDeleted;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public Date getCreatedTime() {
		return createdTime;
	}

	public void setCreatedTime(Date createdTime) {
		this.createdTime = createdTime;
	}

	public Date getModifiedTime() {
		return modifiedTime;
	}

	public void setModifiedTime(Date modifiedTime) {
		this.modifiedTime = modifiedTime;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	@Override
	public String toString() {
		return "Security [securityId=" + securityId + ", securityName=" + securityName + ", isEnable=" + isEnable
				+ ", isDeleted=" + isDeleted + ", description=" + description + ", type=" + type + "]";
	}
	
}
