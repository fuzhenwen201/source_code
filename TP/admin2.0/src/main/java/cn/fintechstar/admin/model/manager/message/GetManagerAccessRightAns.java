package cn.fintechstar.admin.model.manager.message;

import com.google.gson.annotations.SerializedName;

public class GetManagerAccessRightAns {

	@SerializedName("reqid")
	private String reqId;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;
	
	@SerializedName("adminid")
	private Integer adminId;
	
	@SerializedName("managerid")
	private int managerId;
	
	@SerializedName("createuser")
	private int createUser;
	
	@SerializedName("deletetrader")
	private int deleteTrader;
	
	@SerializedName("updatetrader")
	private int updateTrader;
	
	@SerializedName("viewtrader")
	private int viewTrader;
	
	@SerializedName("viewtraderdetails")
	private int viewTraderDetails;
	
	@SerializedName("createmanager")
	private int createManager;
	
	@SerializedName("deletemanager")
	private int deleteManager;
	
	@SerializedName("updatemanager")
	private int updateManager;
	
	@SerializedName("viewmanager")
	private int viewManager;
	
	@SerializedName("viewmanagerdetails")
	private int viewManagerDetails;
	
	@SerializedName("modifymanageraccess")
	private int modifyManagerAccess;
	
	private int credit;
	
	private int balance;
	
	@SerializedName("opentrades")
	private int openTrades;
	
	@SerializedName("bomonitoring")
	private int boMonitoring;

	
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

	public Integer getAdminId() {
		return adminId;
	}

	public void setAdminId(Integer adminId) {
		this.adminId = adminId;
	}

	public int getManagerId() {
		return managerId;
	}

	public void setManagerId(int managerId) {
		this.managerId = managerId;
	}

	public int getCreateUser() {
		return createUser;
	}

	public void setCreateUser(int createUser) {
		this.createUser = createUser;
	}

	public int getDeleteTrader() {
		return deleteTrader;
	}

	public void setDeleteTrader(int deleteTrader) {
		this.deleteTrader = deleteTrader;
	}

	public int getUpdateTrader() {
		return updateTrader;
	}

	public void setUpdateTrader(int updateTrader) {
		this.updateTrader = updateTrader;
	}

	public int getViewTrader() {
		return viewTrader;
	}

	public void setViewTrader(int viewTrader) {
		this.viewTrader = viewTrader;
	}

	public int getViewTraderDetails() {
		return viewTraderDetails;
	}

	public void setViewTraderDetails(int viewTraderDetails) {
		this.viewTraderDetails = viewTraderDetails;
	}

	public int getCreateManager() {
		return createManager;
	}

	public void setCreateManager(int createManager) {
		this.createManager = createManager;
	}

	public int getDeleteManager() {
		return deleteManager;
	}

	public void setDeleteManager(int deleteManager) {
		this.deleteManager = deleteManager;
	}

	public int getUpdateManager() {
		return updateManager;
	}

	public void setUpdateManager(int updateManager) {
		this.updateManager = updateManager;
	}

	public int getViewManager() {
		return viewManager;
	}

	public void setViewManager(int viewManager) {
		this.viewManager = viewManager;
	}

	public int getViewManagerDetails() {
		return viewManagerDetails;
	}

	public void setViewManagerDetails(int viewManagerDetails) {
		this.viewManagerDetails = viewManagerDetails;
	}

	public int getModifyManagerAccess() {
		return modifyManagerAccess;
	}

	public void setModifyManagerAccess(int modifyManagerAccess) {
		this.modifyManagerAccess = modifyManagerAccess;
	}

	public int getCredit() {
		return credit;
	}

	public void setCredit(int credit) {
		this.credit = credit;
	}

	public int getBalance() {
		return balance;
	}

	public void setBalance(int balance) {
		this.balance = balance;
	}

	public int getOpenTrades() {
		return openTrades;
	}

	public void setOpenTrades(int openTrades) {
		this.openTrades = openTrades;
	}

	public int getBoMonitoring() {
		return boMonitoring;
	}

	public void setBoMonitoring(int boMonitoring) {
		this.boMonitoring = boMonitoring;
	}
	
}
