package cn.fintechstar.admin.model.security.message;


import java.util.List;

import com.google.gson.annotations.SerializedName;

import cn.fintechstar.admin.model.security.SecuritySymbol;

public class GetSecuritiesAns {

	@SerializedName("reqid")
	private String reqId;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;
	
	@SerializedName("adminid")
	private int adminId;
	
	@SerializedName("securitydetails")
	private List<SecuritySymbol> securitySymbols;
	
	@SerializedName("symbolist")
	private String symbolList;
	
	private String type;
	
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

	public List<SecuritySymbol> getSecuritySymbols() {
		return securitySymbols;
	}

	public void setSecuritySymbols(List<SecuritySymbol> securitySymbols) {
		this.securitySymbols = securitySymbols;
	}

	public String getSymbolList() {
		return symbolList;
	}

	public void setSymbolList(String symbolList) {
		this.symbolList = symbolList;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public boolean isStatus() {
		return status;
	}

	public void setStatus(boolean status) {
		this.status = status;
	}
	
	
}
