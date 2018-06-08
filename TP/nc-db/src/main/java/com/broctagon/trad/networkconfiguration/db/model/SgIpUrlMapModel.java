package com.broctagon.trad.networkconfiguration.db.model;

public class SgIpUrlMapModel {

	private Integer id;
	private Integer sgid;
	private String sg_internetip;
	private String sg_localip;
	private Integer sg_localport;
	private String url;
	private Integer operator_id;
	private String modify_time;
	private String describe;
		
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public Integer getSgid() {
		return sgid;
	}
	public void setSgid(Integer sgid) {
		this.sgid = sgid;
	}
	public String getSg_internetip() {
		return sg_internetip;
	}
	public void setSg_internetip(String sg_internetip) {
		this.sg_internetip = sg_internetip;
	}
	public String getSg_localip() {
		return sg_localip;
	}
	public void setSg_localip(String sg_localip) {
		this.sg_localip = sg_localip;
	}
	public Integer getSg_localport() {
		return sg_localport;
	}
	public void setSg_localport(Integer sg_localport) {
		this.sg_localport = sg_localport;
	}
	public String getUrl() {
		return url;
	}
	public void setUrl(String url) {
		this.url = url;
	}
	public Integer getOperator_id() {
		return operator_id;
	}
	public void setOperator_id(Integer operator_id) {
		this.operator_id = operator_id;
	}
	public String getModify_time() {
		return modify_time;
	}
	public void setModify_time(String modify_time) {
		this.modify_time = modify_time;
	}
	public String getDescribe() {
		return describe;
	}
	public void setDescribe(String describe) {
		this.describe = describe;
	}
	
}
