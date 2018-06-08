package cn.fintechstar.admin.model.group.message;

import java.util.List;

import com.google.gson.annotations.SerializedName;

import cn.fintechstar.admin.model.group.GroupDetailsVo;

public class GetGroupsAns {

	@SerializedName("reqid")
	private String reqId;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;
	
	@SerializedName("adminid")
	private int adminId;
	
	@SerializedName("groupdetails")
	private List<GroupDetailsVo> groupDetails;
	
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

	public int getAdminId() {
		return adminId;
	}

	public void setAdminId(int adminId) {
		this.adminId = adminId;
	}

	public List<GroupDetailsVo> getGroupDetails() {
		return groupDetails;
	}

	public void setGroupDetails(List<GroupDetailsVo> groupDetails) {
		this.groupDetails = groupDetails;
	}

	public boolean isStatus() {
		return status;
	}

	public void setStatus(boolean status) {
		this.status = status;
	}
}
