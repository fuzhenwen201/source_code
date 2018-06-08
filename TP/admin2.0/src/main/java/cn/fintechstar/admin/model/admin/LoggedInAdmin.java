package cn.fintechstar.admin.model.admin;

import java.util.Date;

public class LoggedInAdmin {

	private Integer id;
	private Integer adminId;
	private String ipAddress;
	private String macAddress;
	private String hardDiskSerialNo;
	private String reqId;
	private String sessionId;
	private Date loginTime;
	private Date logoutTime;

	
	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public Integer getAdminId() {
		return adminId;
	}

	public void setAdminId(Integer adminId) {
		this.adminId = adminId;
	}

	public String getIpAddress() {
		return ipAddress;
	}

	public void setIpAddress(String ipAddress) {
		this.ipAddress = ipAddress;
	}

	public String getMacAddress() {
		return macAddress;
	}

	public void setMacAddress(String macAddress) {
		this.macAddress = macAddress;
	}

	public String getHardDiskSerialNo() {
		return hardDiskSerialNo;
	}

	public void setHardDiskSerialNo(String hardDiskSerialNo) {
		this.hardDiskSerialNo = hardDiskSerialNo;
	}

	public String getReqId() {
		return reqId;
	}

	public void setReqId(String reqId) {
		this.reqId = reqId;
	}

	public String getSessionId() {
		return sessionId;
	}

	public void setSessionId(String sessionId) {
		this.sessionId = sessionId;
	}

	public Date getLoginTime() {
		return loginTime;
	}

	public void setLoginTime(Date loginTime) {
		this.loginTime = loginTime;
	}

	public Date getLogoutTime() {
		return logoutTime;
	}

	public void setLogoutTime(Date logoutTime) {
		this.logoutTime = logoutTime;
	}

	@Override
	public String toString() {
		return "LoggedInAdmin [id=" + id + ", adminId=" + adminId + ", ipAddress=" + ipAddress + ", macAddress="
				+ macAddress + ", hardDiskSerialNo=" + hardDiskSerialNo + ", reqId=" + reqId + ", sessionId="
				+ sessionId + ", loginTime=" + loginTime + ", logoutTime=" + logoutTime + "]";
	}

}
