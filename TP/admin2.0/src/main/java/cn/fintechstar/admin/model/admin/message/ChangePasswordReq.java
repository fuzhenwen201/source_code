package cn.fintechstar.admin.model.admin.message;

import com.google.gson.annotations.SerializedName;

public class ChangePasswordReq {

	@SerializedName("reqid")
	private String reqId;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;
	
	@SerializedName("adminid")
	private int adminid;
	
	private String password;
	
	@SerializedName("oldpassword")
	private String oldPassword;

	
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

	public int getAdminid() {
		return adminid;
	}

	public void setAdminid(int adminid) {
		this.adminid = adminid;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getOldPassword() {
		return oldPassword;
	}

	public void setOldPassword(String oldPassword) {
		this.oldPassword = oldPassword;
	}
	
}
