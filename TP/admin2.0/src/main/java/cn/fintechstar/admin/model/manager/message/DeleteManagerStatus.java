package cn.fintechstar.admin.model.manager.message;

import com.google.gson.annotations.SerializedName;

public class DeleteManagerStatus {

	@SerializedName("managerid")
	private int managerId;
	
	@SerializedName("responsemsg")
	private String responseMsg;
	
	private boolean status;

	public int getManagerId() {
		return managerId;
	}

	public void setManagerId(int managerId) {
		this.managerId = managerId;
	}

	public String getResponseMsg() {
		return responseMsg;
	}

	public void setResponseMsg(String responseMsg) {
		this.responseMsg = responseMsg;
	}

	public boolean isStatus() {
		return status;
	}

	public void setStatus(boolean status) {
		this.status = status;
	}

}
