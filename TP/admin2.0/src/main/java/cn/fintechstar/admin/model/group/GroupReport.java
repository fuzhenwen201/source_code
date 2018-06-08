package cn.fintechstar.admin.model.group;

import java.util.Date;

import com.google.gson.annotations.SerializedName;

public class GroupReport {
	
	@SerializedName("groupid")
	private int groupId	;
	
	@SerializedName("isreport")
	private Boolean isReport;
	
	@SerializedName("smtpserver")
	private String smtpServer;
	
	@SerializedName("templatepath")
	private String templatePath;
	
	@SerializedName("smtplogin")
	private String smtpLogin;
	
	@SerializedName("supportemail")
	private String supportEmail;
	
	@SerializedName("iscopyreport")
	private Boolean isCopyReport;
	
	@SerializedName("signature")
	private String signature;
	
	@SerializedName("modifiedtime")
	private Date modifiedTime;

	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public Boolean getIsReport() {
		return isReport;
	}

	public void setIsReport(Boolean isReport) {
		this.isReport = isReport;
	}

	public String getSmtpServer() {
		return smtpServer;
	}

	public void setSmtpServer(String smtpServer) {
		this.smtpServer = smtpServer;
	}

	public String getTemplatePath() {
		return templatePath;
	}

	public void setTemplatePath(String templatePath) {
		this.templatePath = templatePath;
	}

	public String getSmtpLogin() {
		return smtpLogin;
	}

	public void setSmtpLogin(String smtpLogin) {
		this.smtpLogin = smtpLogin;
	}

	public String getSupportEmail() {
		return supportEmail;
	}

	public void setSupportEmail(String supportEmail) {
		this.supportEmail = supportEmail;
	}

	public Boolean getIsCopyReport() {
		return isCopyReport;
	}

	public void setIsCopyReport(Boolean isCopyReport) {
		this.isCopyReport = isCopyReport;
	}

	public String getSignature() {
		return signature;
	}

	public void setSignature(String signature) {
		this.signature = signature;
	}

	public Date getModifiedTime() {
		return modifiedTime;
	}

	public void setModifiedTime(Date modifiedTime) {
		this.modifiedTime = modifiedTime;
	}

	@Override
	public String toString() {
		return "GroupReport [groupId=" + groupId + ", isReport=" + isReport + ", smtpServer=" + smtpServer
				+ ", templatePath=" + templatePath + ", smtpLogin=" + smtpLogin + ", supportEmail=" + supportEmail
				+ ", isCopyReport=" + isCopyReport + ", signature=" + signature + "]";
	}
	
}
