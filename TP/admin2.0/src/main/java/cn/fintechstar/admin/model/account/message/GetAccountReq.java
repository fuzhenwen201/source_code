package cn.fintechstar.admin.model.account.message;

import com.google.gson.annotations.SerializedName;

public class GetAccountReq {
	
	@SerializedName("reqid")
	private String reqId;

	private String session;

	@SerializedName("sgid")
	private int sgId;

	@SerializedName("adminid")
	private int adminId;

	@SerializedName("clientid")
	private int clientId;

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

	public int getClientId() {
		return clientId;
	}

	public void setClientId(int clientId) {
		this.clientId = clientId;
	}

}