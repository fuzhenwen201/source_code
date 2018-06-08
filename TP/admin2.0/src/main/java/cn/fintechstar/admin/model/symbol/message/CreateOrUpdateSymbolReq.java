package cn.fintechstar.admin.model.symbol.message;

import java.util.List;

import com.google.gson.annotations.SerializedName;

import cn.fintechstar.admin.model.symbol.BoSymbolVo;
import cn.fintechstar.admin.model.symbol.SymbolCalculation;
import cn.fintechstar.admin.model.symbol.SymbolCommon;
import cn.fintechstar.admin.model.symbol.SymbolFilteration;
import cn.fintechstar.admin.model.symbol.SymbolSessionVo;
import cn.fintechstar.admin.model.symbol.SymbolSwap;

public class CreateOrUpdateSymbolReq {

	@SerializedName("reqid")
	private String reqId;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;
	
	@SerializedName("adminid")
	private int adminId;
	
	@SerializedName("symbolid")
	private int symbolId;
	
	@SerializedName("symboldetail")
	private SymbolCommon symbolDetail;
	
	@SerializedName("symbolfilteration")
	private SymbolFilteration symbolFilteration;

	@SerializedName("symbolcalculation")
	private SymbolCalculation symbolCalculation;

	@SerializedName("symbolswap")
	private SymbolSwap symbolSwap;
	
	@SerializedName("sessionList")
	private List<SymbolSessionVo> symbolSessions;
	
	@SerializedName("bosymbol")
	private BoSymbolVo boSymbol;

	
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

	public SymbolCommon getSymbolDetail() {
		return symbolDetail;
	}

	public void setSymbolDetail(SymbolCommon symbolDetail) {
		this.symbolDetail = symbolDetail;
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

	public List<SymbolSessionVo> getSymbolSessions() {
		return symbolSessions;
	}

	public void setSymbolSessions(List<SymbolSessionVo> symbolSessions) {
		this.symbolSessions = symbolSessions;
	}

	public BoSymbolVo getBoSymbol() {
		return boSymbol;
	}

	public void setBoSymbol(BoSymbolVo boSymbol) {
		this.boSymbol = boSymbol;
	}

}
