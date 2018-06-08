package cn.fintechstar.admin.model.symbol.message;

import com.google.gson.annotations.SerializedName;

public class BasicSymbolAns {

	@SerializedName("reqid")
	private String reqId;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;

	@SerializedName("adminid")
	private int adminId;
	
	@SerializedName("symbolid")
	private int symbolId;
	
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

	public int getSymbolId() {
		return symbolId;
	}

	public void setSymbolId(int symbolId) {
		this.symbolId = symbolId;
	}

	public boolean isStatus() {
		return status;
	}

	public void setStatus(boolean status) {
		this.status = status;
	}
	
}
