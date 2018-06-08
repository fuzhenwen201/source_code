package cn.fintechstar.admin.model.synch;

public class SecurityOfSqlite {

	private Integer id;
	private Integer securityId;
	private String securityName;
	private String symbolList;
	private Integer adminId;
	private String modifyTime;
	private String description;

	
	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public Integer getSecurityId() {
		return securityId;
	}

	public void setSecurityId(Integer securityId) {
		this.securityId = securityId;
	}

	public String getSecurityName() {
		return securityName;
	}

	public void setSecurityName(String securityName) {
		this.securityName = securityName;
	}

	public String getSymbolList() {
		return symbolList;
	}

	public void setSymbolList(String symbolList) {
		this.symbolList = symbolList;
	}

	public Integer getAdminId() {
		return adminId;
	}

	public void setAdminId(Integer adminId) {
		this.adminId = adminId;
	}

	public String getModifyTime() {
		return modifyTime;
	}

	public void setModifyTime(String modifyTime) {
		this.modifyTime = modifyTime;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}
	
	
}
