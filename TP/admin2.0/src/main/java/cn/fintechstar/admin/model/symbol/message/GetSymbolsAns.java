package cn.fintechstar.admin.model.symbol.message;

import java.util.List;

import com.google.gson.annotations.SerializedName;

import cn.fintechstar.admin.model.symbol.SymbolVo;

public class GetSymbolsAns {

	@SerializedName("reqid")
	private String reqId;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;
	
	@SerializedName("adminid")
	private int adminId;
	
	private boolean status;
	
	@SerializedName("symboldetails")
	private List<SymbolVo> symbolDetails;

	
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

	public boolean isStatus() {
		return status;
	}

	public void setStatus(boolean status) {
		this.status = status;
	}

	public List<SymbolVo> getSymbolDetails() {
		return symbolDetails;
	}

	public void setSymbolDetails(List<SymbolVo> symbolDetails) {
		this.symbolDetails = symbolDetails;
	}

}
