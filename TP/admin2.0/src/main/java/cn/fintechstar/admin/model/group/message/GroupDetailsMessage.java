package cn.fintechstar.admin.model.group.message;

import java.util.List;

import com.google.gson.annotations.SerializedName;

import cn.fintechstar.admin.model.group.GroupArchieve;
import cn.fintechstar.admin.model.group.GroupCommon;
import cn.fintechstar.admin.model.group.GroupMargin;
import cn.fintechstar.admin.model.group.GroupReport;
import cn.fintechstar.admin.model.group.GroupRight;
import cn.fintechstar.admin.model.group.GroupSecurity;
import cn.fintechstar.admin.model.group.GroupSymbol;
import cn.fintechstar.admin.model.group.TradCondition;

public class GroupDetailsMessage {

	@SerializedName("reqid")
	private String reqId;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;
	
	@SerializedName("groupid")
	private int groupId;
	
	@SerializedName("adminid")
	private int adminId;
	
	@SerializedName("common")
	private GroupCommon groupCommon;
	
	@SerializedName("rights")
	private GroupRight groupRight;
	
	@SerializedName("archieve")
	private GroupArchieve groupArchieve;
	
	@SerializedName("margin")
	private GroupMargin groupMargin;
	
	@SerializedName("securitydetails")
	private List<GroupSecurity> groupSecurities;
	
	@SerializedName("symboldetails")
	private List<GroupSymbol> groupSymbols;

	@SerializedName("tradeconds")
	private TradCondition tradCondition;
	
	@SerializedName("report")
	private GroupReport groupReport;
	
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

	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public int getAdminId() {
		return adminId;
	}

	public void setAdminId(int adminId) {
		this.adminId = adminId;
	}

	public GroupCommon getGroupCommon() {
		return groupCommon;
	}

	public void setGroupCommon(GroupCommon groupCommon) {
		this.groupCommon = groupCommon;
	}

	public GroupRight getGroupRight() {
		return groupRight;
	}

	public void setGroupRight(GroupRight groupRight) {
		this.groupRight = groupRight;
	}

	public GroupArchieve getGroupArchieve() {
		return groupArchieve;
	}

	public void setGroupArchieve(GroupArchieve groupArchieve) {
		this.groupArchieve = groupArchieve;
	}

	public GroupMargin getGroupMargin() {
		return groupMargin;
	}

	public void setGroupMargin(GroupMargin groupMargin) {
		this.groupMargin = groupMargin;
	}

	public List<GroupSecurity> getGroupSecurities() {
		return groupSecurities;
	}

	public void setGroupSecurities(List<GroupSecurity> groupSecurities) {
		this.groupSecurities = groupSecurities;
	}

	public List<GroupSymbol> getGroupSymbols() {
		return groupSymbols;
	}

	public void setGroupSymbols(List<GroupSymbol> groupSymbols) {
		this.groupSymbols = groupSymbols;
	}

	public TradCondition getTradCondition() {
		return tradCondition;
	}

	public void setTradCondition(TradCondition tradCondition) {
		this.tradCondition = tradCondition;
	}

	public GroupReport getGroupReport() {
		return groupReport;
	}

	public void setGroupReport(GroupReport groupReport) {
		this.groupReport = groupReport;
	}

	public boolean isStatus() {
		return status;
	}

	public void setStatus(boolean status) {
		this.status = status;
	}

}
