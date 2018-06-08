package cn.fintechstar.admin.model.admin;

import com.google.gson.annotations.SerializedName;

public class AdminVo {

	@SerializedName("adminid")
	private int adminId;
	
	private String name;
	
	private String password;
	
	private String address;
	
	@SerializedName("phoneno")
	private String phoneNo;

	
	public int getAdminId() {
		return adminId;
	}

	public void setAdminId(int adminId) {
		this.adminId = adminId;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getAddress() {
		return address;
	}

	public void setAddress(String address) {
		this.address = address;
	}

	public String getPhoneNo() {
		return phoneNo;
	}

	public void setPhoneNo(String phoneNo) {
		this.phoneNo = phoneNo;
	}
	
}
