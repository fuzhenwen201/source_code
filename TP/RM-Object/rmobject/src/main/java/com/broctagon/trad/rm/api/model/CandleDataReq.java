/**
 * 
 */
package com.broctagon.trad.rm.api.model;

/**
 * @author imdadullah
 *
 */
public class CandleDataReq {
	private String requestid;
	private int groupid;
	private int clientid;
	private int candlecount;
	private String symbol;
	private String granularity;
	
	public String getRequestid() {
		return requestid;
	}
	public void setRequestid(String requestid) {
		this.requestid = requestid;
	}
	public int getGroupid() {
		return groupid;
	}
	public void setGroupid(int groupid) {
		this.groupid = groupid;
	}
	public int getClientid() {
		return clientid;
	}
	public void setClientid(int clientid) {
		this.clientid = clientid;
	}
	public int getCandlecount() {
		return candlecount;
	}
	public void setCandlecount(int candlecount) {
		this.candlecount = candlecount;
	}
	public String getSymbol() {
		return symbol;
	}
	public void setSymbol(String symbol) {
		this.symbol = symbol;
	}
	public String getGranularity() {
		return granularity;
	}
	public void setGranularity(String granularity) {
		this.granularity = granularity;
	}
	
	
}
