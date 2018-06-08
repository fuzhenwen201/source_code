package cn.fintechstar.admin.model.account;

import com.google.gson.annotations.SerializedName;

public class TraderVo {

	@SerializedName("clientid")
	private int clientId;
	
	@SerializedName("groupname")
	private String groupName;
	
	@SerializedName("firstname")
	private String firstName;
	
	private String city;

	
	public int getClientId() {
		return clientId;
	}

	public void setClientId(int clientId) {
		this.clientId = clientId;
	}

	public String getGroupName() {
		return groupName;
	}

	public void setGroupName(String groupName) {
		this.groupName = groupName;
	}

	public String getFirstName() {
		return firstName;
	}

	public void setFirstName(String firstName) {
		this.firstName = firstName;
	}

	public String getCity() {
		return city;
	}

	public void setCity(String city) {
		this.city = city;
	}
	
}
