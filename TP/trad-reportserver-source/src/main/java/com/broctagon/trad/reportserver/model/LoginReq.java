package com.broctagon.trad.reportserver.model;

public class LoginReq {
	
//	"{\"reqid\":\"88bf9e22c36844c8a740e05e88ed97cc\",\"managerid\":1,\"password\":\"123\",\"macaddress\":\"2C1B0D4CD495\",\"harddiskserialno\":\"3035323042363237393430333130303030303349\"}";
	
	private String reqid;   
	private int managerid;  
	private String password;
	private String macaddress;
	private String harddiskserialno;
		
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getMacaddress() {
		return macaddress;
	}
	public void setMacaddress(String macaddress) {
		this.macaddress = macaddress;
	}
	public String getHarddiskserialno() {
		return harddiskserialno;
	}
	public void setHarddiskserialno(String harddiskserialno) {
		this.harddiskserialno = harddiskserialno;
	}
	
}
