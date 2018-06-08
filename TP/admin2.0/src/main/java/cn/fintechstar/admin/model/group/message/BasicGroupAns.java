package cn.fintechstar.admin.model.group.message;

import com.google.gson.annotations.SerializedName;

public class BasicGroupAns {

	@SerializedName("reqid")
	private String reqId;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;
	
	@SerializedName("groupid")
	private int groupId;
	
	@SerializedName("adminid")
	private int adminId;
	
	@SerializedName("groupname")
	private String groupName;
	
	@SerializedName("ownername")
	private String ownerName;
	
	@SerializedName("calllevel")
	private int calllevel;
	
	@SerializedName("stopoutlevel")
	private int stopOutLevel;
	
	private boolean status;

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

	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public int getAdminId() {
		return adminId;
	}

	public void setAdminId(int adminId) {
		this.adminId = adminId;
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

	public int getCalllevel() {
		return calllevel;
	}

	public void setCalllevel(int calllevel) {
		this.calllevel = calllevel;
	}

	public int getStopOutLevel() {
		return stopOutLevel;
	}

	public void setStopOutLevel(int stopOutLevel) {
		this.stopOutLevel = stopOutLevel;
	}

	public boolean isStatus() {
		return status;
	}

	public void setStatus(boolean status) {
		this.status = status;
	}

}
