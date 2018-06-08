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
	private long balance=0;
	private long prevbalance=0;
	private long changedeposit=0;
	private long margin=0;
	private long freemargin=0;
	private long closebalance=0;
	private long boaccount=0;
	private long fxaccount=0;
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
	public long getBalance() {
		return balance;
	}
	public void setBalance(long balance) {
		this.balance = balance;
	}
	public long getPrevbalance() {
		return prevbalance;
	}
	public void setPrevbalance(long prevbalance) {
		this.prevbalance = prevbalance;
	}
	public long getChangedeposit() {
		return changedeposit;
	}
	public void setChangedeposit(long changedeposit) {
		this.changedeposit = changedeposit;
	}
	public long getMargin() {
		return margin;
	}
	public void setMargin(long margin) {
		this.margin = margin;
	}
	public long getFreemargin() {
		return freemargin;
	}
	public void setFreemargin(long freemargin) {
		this.freemargin = freemargin;
	}
	public long getClosebalance() {
		return closebalance;
	}
	public void setClosebalance(long closebalance) {
		this.closebalance = closebalance;
	}
	public long getBoaccount() {
		return boaccount;
	}
	public void setBoaccount(long boaccount) {
		this.boaccount = boaccount;
	}
	public long getFxaccount() {
		return fxaccount;
	}
	public void setFxaccount(long fxaccount) {
		this.fxaccount = fxaccount;
	}
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
	
}
