package com.broctagon.trad.rm.obj.res.message;

import java.util.List;

import com.broctagon.trad.rm.obj.message.SymbolBoMsg;

/**
 * @author imdadullah
 *
 */
public class SymbolBoResMsg {

	private int managerid;
	private int sgid;
	private String reqid;
	private String session;
	private List<SymbolBoMsg> symbolboans;
	
	public List<SymbolBoMsg> getSymbolboans() {
		return symbolboans;
	}
	public void setSymbolboans(List<SymbolBoMsg> symbolboans) {
		this.symbolboans = symbolboans;
	}
	public int getSgid() {
		return sgid;
	}
	public void setSgid(int sgid) {
		this.sgid = sgid;
	}
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public String getSession() {
		return session;
	}
	public void setSession(String session) {
		this.session = session;
	}
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
}
