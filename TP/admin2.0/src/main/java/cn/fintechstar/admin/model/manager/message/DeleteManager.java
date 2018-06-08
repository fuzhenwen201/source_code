package cn.fintechstar.admin.model.manager.message;

import java.util.List;

import com.google.gson.annotations.SerializedName;

public class DeleteManager {

	@SerializedName("reqid")
	private String reqId;
	
    private String session;
    
    @SerializedName("sgid")
    private int sgId;
	
    @SerializedName("adminid")
	private int adminId;
	
    @SerializedName("deletemanagers")
	private List<DeleteManagerStatus> deleteManagerStatus;

    
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

	public List<DeleteManagerStatus> getDeleteManagerStatus() {
		return deleteManagerStatus;
	}

	public void setDeleteManagerStatus(List<DeleteManagerStatus> deleteManagerStatus) {
		this.deleteManagerStatus = deleteManagerStatus;
	}
}
