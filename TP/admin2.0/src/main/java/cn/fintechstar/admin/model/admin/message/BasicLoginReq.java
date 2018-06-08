package cn.fintechstar.admin.model.admin.message;

import com.google.gson.annotations.SerializedName;

public class BasicLoginReq {

	@SerializedName("reqid")
	private String reqId;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;
	
	@SerializedName("adminid")
	private int adminId;
	
	@SerializedName("groupid")
	private int groupId;
	
	@SerializedName("ipadress")
	private String ipAdress;
	
	private String password;

	@SerializedName("macaddress")
	private String macAddress;

	@SerializedName("hardDiskSerialno")
	private String hardDiskSerialno;

	
	public String getReqId() {
		return reqId;
	}

	public void setReqId(String reqId) {
		this.reqId = reqId;
	}

	public String getSession() {
		return session;
	}

	public void setSession(String session) {
		this.session = session;
	}

	public int getSgId() {
		return sgId;
	}

	public void setSgId(int sgId) {
		this.sgId = sgId;
	}

	public int getAdminId() {
		return adminId;
	}

	public void setAdminId(int adminId) {
		this.adminId = adminId;
	}

	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public String getIpAdress() {
		return ipAdress;
	}

	public void setIpAdress(String ipAdress) {
		this.ipAdress = ipAdress;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getMacAddress() {
		return macAddress;
	}

	public void setMacAddress(String macAddress) {
		this.macAddress = macAddress;
	}

	public String getHardDiskSerialno() {
		return hardDiskSerialno;
	}

	public void setHardDiskSerialno(String hardDiskSerialno) {
		this.hardDiskSerialno = hardDiskSerialno;
	}
	
}
