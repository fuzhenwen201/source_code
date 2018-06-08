package cn.fintechstar.admin.model.group;

import java.util.Date;

import com.google.gson.annotations.SerializedName;

public class GroupSecurity {
	
	@SerializedName("groupid")
	private int groupId;
	
	@SerializedName("securityid")
	private Integer securityId;
	
	@SerializedName("isinstruments")
	private int isInstruments;
	
	@SerializedName("istrade")
	private int isTrade;
	
	@SerializedName("isrequestmode")
	private int isRequestMode;
	
	@SerializedName("executionmode")
	private String executionMode;
	
	@SerializedName("spreaddiff")
	private double spreadDiff;
	
	@SerializedName("maxdeviation")
	private double maxDeviation;
	
	@SerializedName("isfreemargin")
	private int isFreeMargin;
	
	@SerializedName("isfastconfirmation")
	private int isFastConfirmation;
	
	@SerializedName("isclosetwoorder")
	private int isCloseTwoOrder;
	
	@SerializedName("iscloseallorder")
	private int isCloseAllOrder;
	
	@SerializedName("autoclose")
	private String autoClose;
	
	@SerializedName("minlot")
	private int minLot;
	
	@SerializedName("maxlot")
	private int maxLot;
	
	@SerializedName("stepsize")
	private int stepSize;
	
	@SerializedName("stdcommission")
	private double stdCommission;
	
	@SerializedName("taxcommission")
	private double taxCommission;
	
	@SerializedName("agentcommission")
	private double agentCommission;
	
	@SerializedName("isenable")
	private int isEnable;
	
	@SerializedName("modifiedtime")
	private Date modifiedTime;

	
	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(Integer groupId) {
		this.groupId = groupId;
	}

	public Integer getSecurityId() {
		return securityId;
	}

	public void setSecurityId(int securityId) {
		this.securityId = securityId;
	}

	public int getIsInstruments() {
		return isInstruments;
	}

	public void setIsInstruments(int isInstruments) {
		this.isInstruments = isInstruments;
	}

	public int getIsTrade() {
		return isTrade;
	}

	public void setIsTrade(int isTrade) {
		this.isTrade = isTrade;
	}

	public int getIsRequestMode() {
		return isRequestMode;
	}

	public void setIsRequestMode(int isRequestMode) {
		this.isRequestMode = isRequestMode;
	}

	public String getExecutionMode() {
		return executionMode;
	}

	public void setExecutionMode(String executionMode) {
		this.executionMode = executionMode;
	}

	public double getSpreadDiff() {
		return spreadDiff;
	}

	public void setSpreadDiff(double spreadDiff) {
		this.spreadDiff = spreadDiff;
	}

	public double getMaxDeviation() {
		return maxDeviation;
	}

	public void setMaxDeviation(double maxDeviation) {
		this.maxDeviation = maxDeviation;
	}

	public int getIsFreeMargin() {
		return isFreeMargin;
	}

	public void setIsFreeMargin(int isFreeMargin) {
		this.isFreeMargin = isFreeMargin;
	}

	public int getIsFastConfirmation() {
		return isFastConfirmation;
	}

	public void setIsFastConfirmation(int isFastConfirmation) {
		this.isFastConfirmation = isFastConfirmation;
	}

	public int getIsCloseTwoOrder() {
		return isCloseTwoOrder;
	}

	public void setIsCloseTwoOrder(int isCloseTwoOrder) {
		this.isCloseTwoOrder = isCloseTwoOrder;
	}

	public int getIsCloseAllOrder() {
		return isCloseAllOrder;
	}

	public void setIsCloseAllOrder(int isCloseAllOrder) {
		this.isCloseAllOrder = isCloseAllOrder;
	}

	public String getAutoClose() {
		return autoClose;
	}

	public void setAutoClose(String autoClose) {
		this.autoClose = autoClose;
	}

	public int getMinLot() {
		return minLot;
	}

	public void setMinLot(int minLot) {
		this.minLot = minLot;
	}

	public int getMaxLot() {
		return maxLot;
	}

	public void setMaxLot(int maxLot) {
		this.maxLot = maxLot;
	}

	public int getStepSize() {
		return stepSize;
	}

	public void setStepSize(int stepSize) {
		this.stepSize = stepSize;
	}

	public double getStdCommission() {
		return stdCommission;
	}

	public void setStdCommission(double stdCommission) {
		this.stdCommission = stdCommission;
	}

	public double getTaxCommission() {
		return taxCommission;
	}

	public void setTaxCommission(double taxCommission) {
		this.taxCommission = taxCommission;
	}

	public double getAgentCommission() {
		return agentCommission;
	}

	public void setAgentCommission(double agentCommission) {
		this.agentCommission = agentCommission;
	}

	public int getIsEnable() {
		return isEnable;
	}

	public void setIsEnable(int isEnable) {
		this.isEnable = isEnable;
	}

	public Date getModifiedTime() {
		return modifiedTime;
	}

	public void setModifiedTime(Date modifiedTime) {
		this.modifiedTime = modifiedTime;
	}

	@Override
	public String toString() {
		return "GroupSecurity [groupId=" + groupId + ", securityId=" + securityId + ", isInstruments=" + isInstruments
				+ ", isTrade=" + isTrade + ", isRequestMode=" + isRequestMode + ", executionMode=" + executionMode
				+ ", spreadDiff=" + spreadDiff + ", maxDeviation=" + maxDeviation + ", isFreeMargin=" + isFreeMargin
				+ ", isFastConfirmation=" + isFastConfirmation + ", isCloseTwoOrder=" + isCloseTwoOrder
				+ ", isCloseAllOrder=" + isCloseAllOrder + ", autoClose=" + autoClose + ", minLot=" + minLot
				+ ", maxLot=" + maxLot + ", stepSize=" + stepSize + ", stdCommission=" + stdCommission
				+ ", taxCommission=" + taxCommission + ", agentCommission=" + agentCommission + ", isEnable=" + isEnable
				+ "]";
	}

}
