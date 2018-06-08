package cn.fintechstar.admin.model.group;

import java.util.Date;

import com.google.gson.annotations.SerializedName;

public class GroupCommon {

	@SerializedName("groupId")
	private int groupId;
	
	@SerializedName("groupname")
	private String groupName;
	
	@SerializedName("isenable")
	private int isEnable;
	
	private String otp;
	
	@SerializedName("isenableforceotp")
	private int isEnableForceOtp;
	
	@SerializedName("ownername")
	private String ownerName;
	
	@SerializedName("supportedpage")
	private String supportedPage;
	
	@SerializedName("defaultdeposit")
	private double defaultDeposit;
	
	@SerializedName("depositcurrency")
	private String depositCurrency;
	
	@SerializedName("defaultleverage")
	private double defaultLeverage;
	
	@SerializedName("annualinterest")
	private double annualInterest;
	
	@SerializedName("isdeleted")
	private int isDeleted;
	
	@SerializedName("createdtime")
	private Date createdTime;
	
	@SerializedName("modifiedTime")
	private Date modifiedTime;

	
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

	public int getIsEnable() {
		return isEnable;
	}

	public void setIsEnable(int isEnable) {
		this.isEnable = isEnable;
	}

	public String getOtp() {
		return otp;
	}

	public void setOtp(String otp) {
		this.otp = otp;
	}

	public int getIsEnableForceOtp() {
		return isEnableForceOtp;
	}

	public void setIsEnableForceOtp(int isEnableForceOtp) {
		this.isEnableForceOtp = isEnableForceOtp;
	}

	public String getOwnerName() {
		return ownerName;
	}

	public void setOwnerName(String ownerName) {
		this.ownerName = ownerName;
	}

	public String getSupportedPage() {
		return supportedPage;
	}

	public void setSupportedPage(String supportedPage) {
		this.supportedPage = supportedPage;
	}

	public double getDefaultDeposit() {
		return defaultDeposit;
	}

	public void setDefaultDeposit(double defaultDeposit) {
		this.defaultDeposit = defaultDeposit;
	}

	public String getDepositCurrency() {
		return depositCurrency;
	}

	public void setDepositCurrency(String depositCurrency) {
		this.depositCurrency = depositCurrency;
	}

	public double getDefaultLeverage() {
		return defaultLeverage;
	}

	public void setDefaultLeverage(double defaultLeverage) {
		this.defaultLeverage = defaultLeverage;
	}

	public double getAnnualInterest() {
		return annualInterest;
	}

	public void setAnnualInterest(double annualInterest) {
		this.annualInterest = annualInterest;
	}

	public int getIsDeleted() {
		return isDeleted;
	}

	public void setIsDeleted(int isDeleted) {
		this.isDeleted = isDeleted;
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

	@Override
	public String toString() {
		return "GroupCommon [groupId=" + groupId + ", groupName=" + groupName + ", isEnable=" + isEnable + ", otp="
				+ otp + ", isEnableForceOtp=" + isEnableForceOtp + ", ownerName=" + ownerName + ", supportedPage="
				+ supportedPage + ", defaultDeposit=" + defaultDeposit + ", depositCurrency=" + depositCurrency
				+ ", defaultLeverage=" + defaultLeverage + ", annualInterest=" + annualInterest + ", isDeleted="
				+ isDeleted + "]";
	}
	
}
