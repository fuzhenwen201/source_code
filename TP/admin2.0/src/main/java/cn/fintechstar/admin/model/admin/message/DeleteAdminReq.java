package cn.fintechstar.admin.model.admin.message;

import java.util.List;

import com.google.gson.annotations.SerializedName;

public class DeleteAdminReq {

	@SerializedName("reqid")
	private String reqId;
	
	@SerializedName("adminid")
	private int adminId;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;
	
	@SerializedName("adminids")
	private List<Integer> adminIds;

	
	public String getReqId() {
		return reqId;
	}

	public void setReqId(String reqId) {
		this.reqId = reqId;
	}

	public int getAdminId() {
		return adminId;
	}

	public void setAdminId(int adminId) {
		this.adminId = adminId;
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

	public List<Integer> getAdminIds() {
		return adminIds;
	}

	public void setAdminIds(List<Integer> adminIds) {
		this.adminIds = adminIds;
	}
	
}
