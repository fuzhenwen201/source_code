package com.broctagon.trad.networkconfiguration.db.model;

public class GroupSpreadModel {
	
	private Integer id;
	private Integer group_id;
	private String group_name;
	private Integer security_id;
	private Integer security_spread;
	private String time_period;
	private Integer maximum_amounts;
	private Integer minimum_amounts;
	private String group_password;
	private Float profit;
	private Integer operator_id;
	private String modify_time;
	private String describe;
	
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public Integer getGroup_id() {
		return group_id;
	}
	public void setGroup_id(Integer group_id) {
		this.group_id = group_id;
	}
	public String getGroup_name() {
		return group_name;
	}
	public void setGroup_name(String group_name) {
		this.group_name = group_name;
	}
	public Integer getSecurity_id() {
		return security_id;
	}
	public void setSecurity_id(Integer security_id) {
		this.security_id = security_id;
	}
	public Integer getSecurity_spread() {
		return security_spread;
	}
	public void setSecurity_spread(Integer security_spread) {
		this.security_spread = security_spread;
	}
	public String getTime_period() {
		return time_period;
	}
	public void setTime_period(String time_period) {
		this.time_period = time_period;
	}
	public Integer getMaximum_amounts() {
		return maximum_amounts;
	}
	public void setMaximum_amounts(Integer maximum_amounts) {
		this.maximum_amounts = maximum_amounts;
	}
	public Integer getMinimum_amounts() {
		return minimum_amounts;
	}
	public void setMinimum_amounts(Integer minimum_amounts) {
		this.minimum_amounts = minimum_amounts;
	}
	public String getGroup_password() {
		return group_password;
	}
	public void setGroup_password(String group_password) {
		this.group_password = group_password;
	}
	
	public Float getProfit() {
		return profit;
	}
	public void setProfit(Float profit) {
		this.profit = profit;
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
