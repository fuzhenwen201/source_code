package cn.fintechstar.admin.model.account;

import java.util.Date;

public class Account {
	private int accountId;
	private int groupId;
	private int clientId;
	private long balance;
	private long prevBalance;
	private long changeDeposit;
	private long margin;
	private long freeMargin;
	private long closeBalance;
	private int dirtyBit;
	private int boAccount;
	private int fxAccount;
	private Date modifiedTime;

	public int getAccountId() {
		return accountId;
	}

	public void setAccountId(int accountId) {
		this.accountId = accountId;
	}

	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public int getClientId() {
		return clientId;
	}

	public void setClientId(int clientId) {
		this.clientId = clientId;
	}

	public long getBalance() {
		return balance;
	}

	public void setBalance(long balance) {
		this.balance = balance;
	}

	public long getPrevBalance() {
		return prevBalance;
	}

	public void setPrevBalance(long prevBalance) {
		this.prevBalance = prevBalance;
	}

	public long getChangeDeposit() {
		return changeDeposit;
	}

	public void setChangeDeposit(long changeDeposit) {
		this.changeDeposit = changeDeposit;
	}

	public long getMargin() {
		return margin;
	}

	public void setMargin(long margin) {
		this.margin = margin;
	}

	public long getFreeMargin() {
		return freeMargin;
	}

	public void setFreeMargin(long freeMargin) {
		this.freeMargin = freeMargin;
	}

	public long getCloseBalance() {
		return closeBalance;
	}

	public void setCloseBalance(long closeBalance) {
		this.closeBalance = closeBalance;
	}

	public int getDirtyBit() {
		return dirtyBit;
	}

	public void setDirtyBit(int dirtyBit) {
		this.dirtyBit = dirtyBit;
	}

	public int getBoAccount() {
		return boAccount;
	}

	public void setBoAccount(int boAccount) {
		this.boAccount = boAccount;
	}

	public int getFxAccount() {
		return fxAccount;
	}

	public void setFxAccount(int fxAccount) {
		this.fxAccount = fxAccount;
	}

	public Date getModifiedTime() {
		return modifiedTime;
	}

	public void setModifiedTime(Date modifiedTime) {
		this.modifiedTime = modifiedTime;
	}

	@Override
	public String toString() {
		return "Account [accountId=" + accountId + ", groupId=" + groupId + ", clientId=" + clientId + ", balance="
				+ balance + ", prevBalance=" + prevBalance + ", changeDeposit=" + changeDeposit + ", margin=" + margin
				+ ", freeMargin=" + freeMargin + ", closeBalance=" + closeBalance + ", dirtyBit=" + dirtyBit
				+ ", boAccount=" + boAccount + ", fxAccount=" + fxAccount + "]";
	}
}
