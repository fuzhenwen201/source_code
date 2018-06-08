package cn.fintechstar.admin.model.symbol.message;

import java.util.List;

import com.google.gson.annotations.SerializedName;

import cn.fintechstar.admin.model.symbol.BoSymbol;
import cn.fintechstar.admin.model.symbol.SymbolCalculation;
import cn.fintechstar.admin.model.symbol.SymbolCommon;
import cn.fintechstar.admin.model.symbol.SymbolFilteration;
import cn.fintechstar.admin.model.symbol.SymbolSession;
import cn.fintechstar.admin.model.symbol.SymbolSwap;

public class GetSymbolByIdAns {

	@SerializedName("reqid")
	private String reqId;

	private String session;

	@SerializedName("sgid")
	private int sgId;

	@SerializedName("adminid")
	private int adminId;

	@SerializedName("symbolid")
	private int symbolId;

	@SerializedName("symboldetails")
	private SymbolCommon symbolDetails;

	@SerializedName("symbolfileteration")
	private SymbolFilteration symbolFilteration;

	@SerializedName("symbolcalculation")
	private SymbolCalculation symbolCalculation;

	@SerializedName("symbolswap")
	private SymbolSwap symbolSwap;

	@SerializedName("symbolsessionlist")
	private List<SymbolSession> symbolSessions;

	private BoSymbol boSymbol;
	
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

	public SymbolCommon getSymbolDetails() {
		return symbolDetails;
	}

	public void setSymbolDetails(SymbolCommon symbolDetails) {
		this.symbolDetails = symbolDetails;
	}

	public SymbolFilteration getSymbolFilteration() {
		return symbolFilteration;
	}

	public void setSymbolFilteration(SymbolFilteration symbolFilteration) {
		this.symbolFilteration = symbolFilteration;
	}

	public SymbolCalculation getSymbolCalculation() {
		return symbolCalculation;
	}

	public void setSymbolCalculation(SymbolCalculation symbolCalculation) {
		this.symbolCalculation = symbolCalculation;
	}

	public SymbolSwap getSymbolSwap() {
		return symbolSwap;
	}

	public void setSymbolSwap(SymbolSwap symbolSwap) {
		this.symbolSwap = symbolSwap;
	}

	public List<SymbolSession> getSymbolSessions() {
		return symbolSessions;
	}

	public void setSymbolSessions(List<SymbolSession> symbolSessions) {
		this.symbolSessions = symbolSessions;
	}

	public BoSymbol getBoSymbol() {
		return boSymbol;
	}

	public void setBoSymbol(BoSymbol boSymbol) {
		this.boSymbol = boSymbol;
	}

	public boolean isStatus() {
		return status;
	}

	public void setStatus(boolean status) {
		this.status = status;
	}
}
