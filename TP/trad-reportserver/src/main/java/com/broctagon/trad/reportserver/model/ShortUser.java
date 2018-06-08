package com.broctagon.trad.reportserver.model;

import java.math.BigDecimal;
import java.sql.Timestamp;

public class ShortUser {

/*	#{user.login},#{user.leverage}, #{user.balance}, #{user.prevMonthBalance}, #{user.prevBalance}, #{user.credit},
	#{user.taxes}, #{user.margin}, #{user.margin_level}, #{user.margin_free}, #{user.currency}*/
	
	private Integer login;	
	private Integer leverage;
	private double balance;		
	private double prevMonthBalance;
	private double prevBalance;
	private double credit;
	private double taxes;
	private double margin;
	private double margin_level;
	private double margin_free;
	private String currency;
	
	public Integer getLogin() {
		return login;
	}
	public void setLogin(Integer login) {
		this.login = login;
	}
	public Integer getLeverage() {
		return leverage;
	}
	public void setLeverage(Integer leverage) {
		this.leverage = leverage;
	}
	public double getBalance() {
		return balance;
	}
	public void setBalance(double balance) {
		this.balance = balance;
	}
	public double getPrevMonthBalance() {
		return prevMonthBalance;
	}
	public void setPrevMonthBalance(double prevMonthBalance) {
		this.prevMonthBalance = prevMonthBalance;
	}
	public double getPrevBalance() {
		return prevBalance;
	}
	public void setPrevBalance(double prevBalance) {
		this.prevBalance = prevBalance;
	}
	public double getCredit() {
		return credit;
	}
	public void setCredit(double credit) {
		this.credit = credit;
	}
	public double getTaxes() {
		return taxes;
	}
	public void setTaxes(double taxes) {
		this.taxes = taxes;
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
	public String getCurrency() {
		return currency;
	}
	public void setCurrency(String currency) {
		this.currency = currency;
	}
	
}
