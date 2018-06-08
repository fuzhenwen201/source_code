package com.broctagon.trad.reportserver.model;

public class Trader {

/*	{\"clientid\":1,"
			+ "\"groupid\":0,\"groupname\":\"Fox\",\"password\":\"2\",\"firstname\":\"wang\",\"lastname\":\"shangxiu\",\"email\":\"dsd@dm.com\",\
		"address\":\"baoan\",\"city\":\"ShenZhen\",\"state\":\"guangdong\",\"country\":\"Brazil\",\"pin\":\"13123123456\",\"accountid\":138,\"balance\":1200,\"prevbalance\":0,\
		"changedeposit\":1200,\"margin\":0,\"freemargin\":1200,\"closebalance\":0,\"boaccount\":0,\"fxaccount\":0,\"managerid\":0},
*/	
	
	private int clientid=0;
	private int groupid=0;
	private String groupname="groupname";
	private String password="";
	private String firstname="";
	private String lastname="";
	private String email="";
	private String address="";
	private String city="";
	private String state="";
	private String country="";
	private String pin="";
	private int accountid=0;
	private int balance=0;
	private int prevbalance=0;
	private int changedeposit=0;
	private int margin=0;
	private int freemargin=0;
	private int closebalance=0;
	private int boaccount=0;
	private int fxaccount=0;
	private int managerid=0;
	
	public int getClientid() {
		return clientid;
	}
	public void setClientid(int clientid) {
		this.clientid = clientid;
	}
	public int getGroupid() {
		return groupid;
	}
	public void setGroupid(int groupid) {
		this.groupid = groupid;
	}
	public String getGroupname() {
		return groupname;
	}
	public void setGroupname(String groupname) {
		this.groupname = groupname;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getFirstname() {
		return firstname;
	}
	public void setFirstname(String firstname) {
		this.firstname = firstname;
	}
	public String getLastname() {
		return lastname;
	}
	public void setLastname(String lastname) {
		this.lastname = lastname;
	}
	public String getEmail() {
		return email;
	}
	public void setEmail(String email) {
		this.email = email;
	}
	public String getAddress() {
		return address;
	}
	public void setAddress(String address) {
		this.address = address;
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
	public String getCountry() {
		return country;
	}
	public void setCountry(String country) {
		this.country = country;
	}
	public String getPin() {
		return pin;
	}
	public void setPin(String pin) {
		this.pin = pin;
	}
	public int getAccountid() {
		return accountid;
	}
	public void setAccountid(int accountid) {
		this.accountid = accountid;
	}
	public int getBalance() {
		return balance;
	}
	public void setBalance(int balance) {
		this.balance = balance;
	}
	public int getPrevbalance() {
		return prevbalance;
	}
	public void setPrevbalance(int prevbalance) {
		this.prevbalance = prevbalance;
	}
	public int getChangedeposit() {
		return changedeposit;
	}
	public void setChangedeposit(int changedeposit) {
		this.changedeposit = changedeposit;
	}
	public int getMargin() {
		return margin;
	}
	public void setMargin(int margin) {
		this.margin = margin;
	}
	public int getFreemargin() {
		return freemargin;
	}
	public void setFreemargin(int freemargin) {
		this.freemargin = freemargin;
	}
	public int getClosebalance() {
		return closebalance;
	}
	public void setClosebalance(int closebalance) {
		this.closebalance = closebalance;
	}
	public int getBoaccount() {
		return boaccount;
	}
	public void setBoaccount(int boaccount) {
		this.boaccount = boaccount;
	}
	public int getFxaccount() {
		return fxaccount;
	}
	public void setFxaccount(int fxaccount) {
		this.fxaccount = fxaccount;
	}
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
		
	
}
