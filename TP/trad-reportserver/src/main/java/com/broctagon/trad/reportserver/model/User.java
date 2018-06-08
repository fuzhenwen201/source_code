package com.broctagon.trad.reportserver.model;

import java.math.BigDecimal;
import java.sql.Timestamp;
import java.util.Arrays;

public class User {

	private Integer login;
	private String group;
	private int enable;
	private int enable_change_pass;
	private int enable_readOnly;
	private int enableOpt;
	private String password_phone;
	private String name;
	private String country;
	private String city;
	private String state;
	private String zipCode;
	private String address;
	private String lead_source;
	private String phone;
	private String email;
	private String comment;
	private String id;
	private String status;
	private Timestamp regDate;   
	private Timestamp lastDate;   
	private Integer leverage;
	private Integer agent_account;
	private Integer timestamp;
	private double balance;		
	private double prevMonthBalance;
	private double prevBalance;
	private double credit;
	private double interestrate;
	private double taxes;
	private Integer send_report;
	private BigDecimal mqid;
	private Integer userColor;
	private double equity;
	private double margin;
	private double margin_level;
	private double margin_free;
	private String currency;
	private byte[] api_data;
	private Timestamp modify_time;
	
	
	public Integer getLogin() {
		return login;
	}
	
	public void setLogin(Integer login) {
		this.login = login;
	}
	
	public String getGroup() {
		return group;
	}
	
	public void setGroup(String group) {
		this.group = group;
	}
	
	public int getEnable() {
		return enable;
	}
	
	public void setEnable(int enable) {
		this.enable = enable;
	}
	
	public int getEnable_change_pass() {
		return enable_change_pass;
	}
	
	public void setEnable_change_pass(int enable_change_pass) {
		this.enable_change_pass = enable_change_pass;
	}
	
	public int getEnable_readOnly() {
		return enable_readOnly;
	}
	
	public void setEnable_readOnly(int enable_readOnly) {
		this.enable_readOnly = enable_readOnly;
	}
	
	public int getEnableOpt() {
		return enableOpt;
	}
	
	public void setEnableOpt(int enableOpt) {
		this.enableOpt = enableOpt;
	}
	
	public String getPassword_phone() {
		return password_phone;
	}
	
	public void setPassword_phone(String password_phone) {
		this.password_phone = password_phone;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public String getCountry() {
		return country;
	}
	
	public void setCountry(String country) {
		this.country = country;
	}
	
	public String getCity() {
		return city;
	}
	
	public void setCity(String city) {
		this.city = city;
	}
	
	public String getState() {
		return state;
	}
	
	public void setState(String state) {
		this.state = state;
	}
	
	public String getZipCode() {
		return zipCode;
	}
	
	public void setZipCode(String zipCode) {
		this.zipCode = zipCode;
	}
	
	public String getAddress() {
		return address;
	}
	
	public void setAddress(String address) {
		this.address = address;
	}
	
	public String getLead_source() {
		return lead_source;
	}
	
	public void setLead_source(String lead_source) {
		this.lead_source = lead_source;
	}
	
	public String getPhone() {
		return phone;
	}
	
	public void setPhone(String phone) {
		this.phone = phone;
	}
	
	public String getEmail() {
		return email;
	}
	
	public void setEmail(String email) {
		this.email = email;
	}
	
	public String getComment() {
		return comment;
	}
	
	public void setComment(String comment) {
		this.comment = comment;
	}
	
	public String getId() {
		return id;
	}
	
	public void setId(String id) {
		this.id = id;
	}
	
	public String getStatus() {
		return status;
	}
	
	public void setStatus(String status) {
		this.status = status;
	}
	
	public Timestamp getRegDate() {
		return regDate;
	}
	
	public void setRegDate(Timestamp regDate) {
		this.regDate = regDate;
	}
	
	public Timestamp getLastDate() {
		return lastDate;
	}
	
	public void setLastDate(Timestamp lastDate) {
		this.lastDate = lastDate;
	}
	
	public Integer getLeverage() {
		return leverage;
	}
	
	public void setLeverage(Integer leverage) {
		this.leverage = leverage;
	}
	
	public Integer getAgent_account() {
		return agent_account;
	}
	
	public void setAgent_account(Integer agent_account) {
		this.agent_account = agent_account;
	}
	
	public Integer getTimestamp() {
		return timestamp;
	}
	
	public void setTimestamp(Integer timestamp) {
		this.timestamp = timestamp;
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
	
	public double getInterestrate() {
		return interestrate;
	}
	
	public void setInterestrate(double interestrate) {
		this.interestrate = interestrate;
	}
	
	public double getTaxes() {
		return taxes;
	}
	
	public void setTaxes(double taxes) {
		this.taxes = taxes;
	}
	
	public Integer getSend_report() {
		return send_report;
	}
	
	public void setSend_report(Integer send_report) {
		this.send_report = send_report;
	}
	
	public BigDecimal getMqid() {
		return mqid;
	}
	
	public void setMqid(BigDecimal mqid) {
		this.mqid = mqid;
	}
	
	public Integer getUserColor() {
		return userColor;
	}
	
	public void setUserColor(Integer userColor) {
		this.userColor = userColor;
	}
	
	public double getEquity() {
		return equity;
	}
	
	public void setEquity(double equity) {
		this.equity = equity;
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

	public byte[] getApi_data() {
		return api_data;
	}

	public void setApi_data(byte[] api_data) {
		this.api_data = api_data;
	}

	public Timestamp getModify_time() {
		return modify_time;
	}
	
	public void setModify_time(Timestamp modify_time) {
		this.modify_time = modify_time;
	}

	@Override
	public String toString() {
		return "User [login=" + login + ", group=" + group + ", enable=" + enable + ", enable_change_pass="
				+ enable_change_pass + ", enable_readOnly=" + enable_readOnly + ", enableOpt=" + enableOpt
				+ ", password_phone=" + password_phone + ", name=" + name + ", country=" + country + ", city=" + city
				+ ", state=" + state + ", zipCode=" + zipCode + ", address=" + address + ", lead_source=" + lead_source
				+ ", phone=" + phone + ", email=" + email + ", comment=" + comment + ", id=" + id + ", status=" + status
				+ ", regDate=" + regDate + ", lastDate=" + lastDate + ", leverage=" + leverage + ", agent_account="
				+ agent_account + ", timestamp=" + timestamp + ", balance=" + balance + ", prevMonthBalance="
				+ prevMonthBalance + ", prevBalance=" + prevBalance + ", credit=" + credit + ", interestrate="
				+ interestrate + ", taxes=" + taxes + ", send_report=" + send_report + ", mqid=" + mqid + ", userColor="
				+ userColor + ", equity=" + equity + ", margin=" + margin + ", margin_level=" + margin_level
				+ ", margin_free=" + margin_free + ", currency=" + currency + ", api_data=" + Arrays.toString(api_data)
				+ ", modify_time=" + modify_time + "]";
	}

}
