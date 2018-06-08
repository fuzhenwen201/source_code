package cn.fintechstar.admin.model.admin;

public class OnlineAdmin {

	private String reqId;
	private String session;
	private int sgId;
	private String ipAddress;
	private String macAddress;
	private String hardDiskSerialno;
	private Admin admin;

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

	public String getIpAddress() {
		return ipAddress;
	}

	public void setIpAddress(String ipAddress) {
		this.ipAddress = ipAddress;
	}

	public String getMacAddress() {
		return macAddress;
	}

	public void setMacAddress(String macAddress) {
		this.macAddress = macAddress;
	}

	public String getHardDiskSerialno() {
		return hardDiskSerialno;
	}

	public void setHardDiskSerialno(String hardDiskSerialno) {
		this.hardDiskSerialno = hardDiskSerialno;
	}

	public Admin getAdmin() {
		return admin;
	}

	public void setAdmin(Admin admin) {
		this.admin = admin;
	}

}
