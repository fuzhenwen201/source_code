package com.broctagon.trad.rm.obj.message;

import java.util.List;

import com.broctagon.trad.rm.obj.model.BalanceTrader;

public class BalanceTradersAns {
	
	private String reqid;
	private List<BalanceTrader> traderlist;
	private String session = "ceaf6bf2634011e7aaf71b925476a83c";
	private int sgid = 6;
	private int managerid = 1;
	
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public List<BalanceTrader> getTraderlist() {
		return traderlist;
	}
	public void setTraderlist(List<BalanceTrader> traderlist) {
		this.traderlist = traderlist;
	}
	public String getSession() {
		return session;
	}
	public void setSession(String session) {
		this.session = session;
	}
	public int getSgid() {
		return sgid;
	}
	public void setSgid(int sgid) {
		this.sgid = sgid;
	}
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
	
}
