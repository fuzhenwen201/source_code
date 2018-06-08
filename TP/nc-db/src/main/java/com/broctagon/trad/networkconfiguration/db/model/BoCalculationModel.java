package com.broctagon.trad.networkconfiguration.db.model;

public class BoCalculationModel {
	
	private Integer id;
	
	private Integer security_id;
	private String security_name;
	private String symbol_list;
	
	private Integer operator_id;
	private String modify_time;
	private String describe;
	
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}	
		
	public Integer getSecurity_id() {
		return security_id;
	}
	public void setSecurity_id(Integer security_id) {
		this.security_id = security_id;
	}
	public String getSecurity_name() {
		return security_name;
	}
	public void setSecurity_name(String security_name) {
		this.security_name = security_name;
	}
	public String getSymbol_list() {
		return symbol_list;
	}
	public void setSymbol_list(String symbol_list) {
		this.symbol_list = symbol_list;
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
