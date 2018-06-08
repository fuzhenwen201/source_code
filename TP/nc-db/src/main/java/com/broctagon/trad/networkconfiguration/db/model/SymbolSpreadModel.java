package com.broctagon.trad.networkconfiguration.db.model;

public class SymbolSpreadModel {
	
	private Integer id;
	private String symbol;
	private Integer symbol_spread;
	private Integer operator_id;
	private String modify_time;
	private String describe;
	
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public String getSymbol() {
		return symbol;
	}
	public void setSymbol(String symbol) {
		this.symbol = symbol;
	}
	public Integer getSymbol_spread() {
		return symbol_spread;
	}
	public void setSymbol_spread(Integer symbol_spread) {
		this.symbol_spread = symbol_spread;
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
