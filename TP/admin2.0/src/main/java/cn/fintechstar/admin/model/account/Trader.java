package cn.fintechstar.admin.model.account;

import java.util.Date;

public class Trader {

	private int clientId;
	private int groupId;
	private String password;
	private String firstName;
	private String lastName;
	private String email;
	private String address;
	private String city;
	private String state;
	private String country;
	private String pin;
	private Boolean isDeleted;
	private Boolean isLogin;
	private Date createdDate;
	private Date updatedDate;
	private int dirtyBit;
	private Boolean isEnable;
	private int metaQuotesId;

	// these are the new additional fields
	private Date lastLoginTime;
	private String traderComment;
	private String phone;
	private String phonePassword;
	private Boolean isAllowChangePassword;
	private Boolean isReadOnly;
	private Boolean isEnableOTP;
	private String investorPassword;
	private int idNumber;
	private Integer leverage;
	private double taxRate;
	private Integer agentAccount;
	private Boolean isSendReport;
	private String leadSource;
	private String status;

	public int getClientId() {
		return clientId;
	}

	public void setClientId(int clientId) {
		this.clientId = clientId;
	}

	public int getGroupId() {
		return groupId;
	}

	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getFirstName() {
		return firstName;
	}

	public void setFirstName(String firstName) {
		this.firstName = firstName;
	}

	public String getLastName() {
		return lastName;
	}

	public void setLastName(String lastName) {
		this.lastName = lastName;
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

	public Boolean getIsDeleted() {
		return isDeleted;
	}

	public void setIsDeleted(Boolean isDeleted) {
		this.isDeleted = isDeleted;
	}

	public Boolean getIsLogin() {
		return isLogin;
	}

	public void setIsLogin(Boolean isLogin) {
		this.isLogin = isLogin;
	}

	public Date getCreatedDate() {
		return createdDate;
	}

	public void setCreatedDate(Date createdDate) {
		this.createdDate = createdDate;
	}

	public Date getUpdatedDate() {
		return updatedDate;
	}

	public void setUpdatedDate(Date updatedDate) {
		this.updatedDate = updatedDate;
	}

	public int getDirtyBit() {
		return dirtyBit;
	}

	public void setDirtyBit(int dirtyBit) {
		this.dirtyBit = dirtyBit;
	}

	public Boolean getIsEnable() {
		return isEnable;
	}

	public void setIsEnable(Boolean isEnable) {
		this.isEnable = isEnable;
	}

	public int getMetaQuotesId() {
		return metaQuotesId;
	}

	public void setMetaQuotesId(int metaQuotesId) {
		this.metaQuotesId = metaQuotesId;
	}

	public Date getLastLoginTime() {
		return lastLoginTime;
	}

	public void setLastLoginTime(Date lastLoginTime) {
		this.lastLoginTime = lastLoginTime;
	}

	public String getTraderComment() {
		return traderComment;
	}

	public void setTraderComment(String traderComment) {
		this.traderComment = traderComment;
	}

	public String getPhone() {
		return phone;
	}

	public void setPhone(String phone) {
		this.phone = phone;
	}

	public String getPhonePassword() {
		return phonePassword;
	}

	public void setPhonePassword(String phonePassword) {
		this.phonePassword = phonePassword;
	}

	public Boolean getIsAllowChangePassword() {
		return isAllowChangePassword;
	}

	public void setIsAllowChangePassword(Boolean isAllowChangePassword) {
		this.isAllowChangePassword = isAllowChangePassword;
	}

	public Boolean getIsReadOnly() {
		return isReadOnly;
	}

	public void setIsReadOnly(Boolean isReadOnly) {
		this.isReadOnly = isReadOnly;
	}

	public Boolean getIsEnableOTP() {
		return isEnableOTP;
	}

	public void setIsEnableOTP(Boolean isEnableOTP) {
		this.isEnableOTP = isEnableOTP;
	}

	public String getInvestorPassword() {
		return investorPassword;
	}

	public void setInvestorPassword(String investorPassword) {
		this.investorPassword = investorPassword;
	}

	public int getIdNumber() {
		return idNumber;
	}

	public void setIdNumber(int idNumber) {
		this.idNumber = idNumber;
	}

	public Integer getLeverage() {
		return leverage;
	}

	public void setLeverage(Integer leverage) {
		this.leverage = leverage;
	}

	public double getTaxRate() {
		return taxRate;
	}

	public void setTaxRate(double taxRate) {
		this.taxRate = taxRate;
	}

	public Integer getAgentAccount() {
		return agentAccount;
	}

	public void setAgentAccount(Integer agentAccount) {
		this.agentAccount = agentAccount;
	}

	public Boolean getIsSendReport() {
		return isSendReport;
	}

	public void setIsSendReport(Boolean isSendReport) {
		this.isSendReport = isSendReport;
	}

	public String getLeadSource() {
		return leadSource;
	}

	public void setLeadSource(String leadSource) {
		this.leadSource = leadSource;
	}

	public String getStatus() {
		return status;
	}

	public void setStatus(String status) {
		this.status = status;
	}

	@Override
	public String toString() {
		return "Trader [clientId=" + clientId + ", groupId=" + groupId + ", password=" + password + ", firstName="
				+ firstName + ", lastName=" + lastName + ", email=" + email + ", address=" + address + ", city=" + city
				+ ", state=" + state + ", country=" + country + ", pin=" + pin + ", isDeleted=" + isDeleted
				+ ", isLogin=" + isLogin + ", dirtyBit=" + dirtyBit + ", isEnable=" + isEnable + ", metaQuotesId="
				+ metaQuotesId + ", traderComment=" + traderComment + ", phone=" + phone + ", phonePassword="
				+ phonePassword + ", isAllowChangePassword=" + isAllowChangePassword + ", isReadOnly=" + isReadOnly
				+ ", isEnableOTP=" + isEnableOTP + ", investorPassword=" + investorPassword + ", idNumber=" + idNumber
				+ ", leverage=" + leverage + ", taxRate=" + taxRate + ", agentAccount=" + agentAccount
				+ ", isSendReport=" + isSendReport + ", leadSource=" + leadSource + ", status=" + status + "]";
	}

}
