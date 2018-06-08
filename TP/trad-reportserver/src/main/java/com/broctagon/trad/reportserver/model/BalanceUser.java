package com.broctagon.trad.reportserver.model;

public class BalanceUser {
	
	private Integer login;	//clientid;
	private double balance;		//  balance;
	private double prevBalance;  // prevbalance;
	private double margin;     // margin;
	private double margin_level;  
	private double margin_free;  // freemargin;
	
	public Integer getLogin() {
		return login;
	}
	public void setLogin(Integer login) {
		this.login = login;
	}
	public double getBalance() {
		return balance;
	}
	public void setBalance(double balance) {
		this.balance = balance;
	}
	public double getPrevBalance() {
		return prevBalance;
	}
	public void setPrevBalance(double prevBalance) {
		this.prevBalance = prevBalance;
	}
	public double getMargin() {
		return margin;
	}
	public void setMargin(double margin) {
		this.margin = margin;
	}
	public double getMargin_level() {
		return margin_level;
	}
	public void setMargin_level(double margin_level) {
		this.margin_level = margin_level;
	}
	public double getMargin_free() {
		return margin_free;
	}
	public void setMargin_free(double margin_free) {
		this.margin_free = margin_free;
	}
	
}
