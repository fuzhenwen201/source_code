package cn.fintechstar.admin.model.account.message;

import com.google.gson.annotations.SerializedName;

public class BasicAccount {
	
	@SerializedName("reqid")
	private String reqId;
	
	@SerializedName("adminid")
	private int adminId ;
	
	private String session;
	
	@SerializedName("sgid")
	private int sgId;


	// Trader information
	@SerializedName("clientid")
	private int clientId;
	
	@SerializedName("groupid")
	private int groupId; 
	
	@SerializedName("groupname")
	private String groupName;
	
	private String password;
	
	@SerializedName("groupid")
	private String firstname;
	
	@SerializedName("groupid")
	private String lastname;
	
	private String email;
	
	private String address;
	
	private String city;

	private String state;
	
	private String country;
	
	private String pin;
	
	@SerializedName("isenable")
	private Boolean isEnable;
	
	@SerializedName("dirtybit")
	private Integer dirtyBit;
	
	
	//Add More Parameters 
	@SerializedName("investorpassword")
	private String investorPassword;
	
	private String phone;
	
	@SerializedName("idnumber")
	private int idNumber;
	
	@SerializedName("phonepassword")
	private String phonePassword;
	
	@SerializedName("tradercomment")
	private String traderComment;
	
	private Integer leverage;
	
	@SerializedName("taxrate")
	private double taxRate;
	
	@SerializedName("agentaccount")
	private Integer agentAccount;
	
	@SerializedName("isallowchangepassword")
	private Boolean isAllowChangePassword;
	
	@SerializedName("isenableotp")
	private Boolean isEnableOtp;
	
	@SerializedName("isreadonly")
	private Boolean isReadonly;
	
	@SerializedName("issendreport")
	private Boolean isSendReport;

	@SerializedName("leadsource")
	private String leadSource;
	
	private String status;
	
	@SerializedName("metaquotesid")
	private int metaQuotesId;
	
	private Boolean isAdmin;
	
	// Acount information
	@SerializedName("accountid")
	private int accountId;
	
	private long balance;
	
	@SerializedName("prevbalance")
	private long prevBalance;
	
	@SerializedName("changedeposit")
	private long changeDeposit;
	
	private long margin;	
	
	@SerializedName("freemargin")
	private long freeMargin;	
	
	@SerializedName("closebalance")
	private long closeBalance;
	
	@SerializedName("boaccount")
	private int boAccount;
	
	@SerializedName("fxaccount")
	private int fxAccount;
	
	@SerializedName("managerid")
	private int managerId;

	public String getReqId() {
		return reqId;
	}

	public void setReqId(String reqId) {
		this.reqId = reqId;
	}

	public int getAdminId() {
		return adminId;
	}

	public void setAdminId(int adminId) {
		this.adminId = adminId;
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

	public String getGroupName() {
		return groupName;
	}

	public void setGroupName(String groupName) {
		this.groupName = groupName;
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

	public Boolean getIsEnable() {
		return isEnable;
	}

	public void setIsEnable(Boolean isEnable) {
		this.isEnable = isEnable;
	}

	public Integer getDirtyBit() {
		return dirtyBit;
	}

	public void setDirtyBit(Integer dirtyBit) {
		this.dirtyBit = dirtyBit;
	}

	public String getInvestorPassword() {
		return investorPassword;
	}

	public void setInvestorPassword(String investorPassword) {
		this.investorPassword = investorPassword;
	}

	public String getPhone() {
		return phone;
	}

	public void setPhone(String phone) {
		this.phone = phone;
	}

	public int getIdNumber() {
		return idNumber;
	}

	public void setIdNumber(int idNumber) {
		this.idNumber = idNumber;
	}

	public String getPhonePassword() {
		return phonePassword;
	}

	public void setPhonePassword(String phonePassword) {
		this.phonePassword = phonePassword;
	}

	public String getTraderComment() {
		return traderComment;
	}

	public void setTraderComment(String traderComment) {
		this.traderComment = traderComment;
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

	public Boolean getIsAllowChangePassword() {
		return isAllowChangePassword;
	}

	public void setIsAllowChangePassword(Boolean isAllowChangePassword) {
		this.isAllowChangePassword = isAllowChangePassword;
	}

	public Boolean getIsEnableOtp() {
		return isEnableOtp;
	}

	public void setIsEnableOtp(Boolean isEnableOtp) {
		this.isEnableOtp = isEnableOtp;
	}

	public Boolean getIsReadonly() {
		return isReadonly;
	}

	public void setIsReadonly(Boolean isReadonly) {
		this.isReadonly = isReadonly;
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

	public int getMetaQuotesId() {
		return metaQuotesId;
	}

	public void setMetaQuotesId(int metaQuotesId) {
		this.metaQuotesId = metaQuotesId;
	}

	public Boolean getIsAdmin() {
		return isAdmin;
	}

	public void setIsAdmin(Boolean isAdmin) {
		this.isAdmin = isAdmin;
	}

	public int getAccountId() {
		return accountId;
	}

	public void setAccountId(int accountId) {
		this.accountId = accountId;
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

	public int getManagerId() {
		return managerId;
	}

	public void setManagerId(int managerId) {
		this.managerId = managerId;
	}
	
}
