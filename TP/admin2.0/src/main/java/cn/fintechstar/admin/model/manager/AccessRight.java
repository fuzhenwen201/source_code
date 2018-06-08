package cn.fintechstar.admin.model.manager;

public class AccessRight {

	private int managerId;
	private int createUser;
	private int deleteTrader;
	private int updateTrader;
	private int viewTrader;
	private int viewTraderDetails;
	private int createManager;
	private int deleteManager;
	private int updateManager;
	private int viewManager;
	private int viewManagerDetails;
	private int modifyManagerAccess;
	private int credit;
	private int balance;
	private int openTrades;
	private int boMonitoring;

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

	@Override
	public String toString() {
		return "AccessRight [managerId=" + managerId + ", createUser=" + createUser + ", deleteTrader=" + deleteTrader
				+ ", updateTrader=" + updateTrader + ", viewTrader=" + viewTrader + ", viewTraderDetails="
				+ viewTraderDetails + ", createManager=" + createManager + ", deleteManager=" + deleteManager
				+ ", updateManager=" + updateManager + ", viewManager=" + viewManager + ", viewManagerDetails="
				+ viewManagerDetails + ", modifyManagerAccess=" + modifyManagerAccess + ", credit=" + credit
				+ ", balance=" + balance + ", openTrades=" + openTrades + ", boMonitoring=" + boMonitoring + "]";
	}

}
