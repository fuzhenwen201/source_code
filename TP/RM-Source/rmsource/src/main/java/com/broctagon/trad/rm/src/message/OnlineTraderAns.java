package com.broctagon.trad.rm.src.message;

import java.util.List;

/**
 * @author kanhaiya
 *
 */
public class OnlineTraderAns {
	private String reqid;
	private int managerid;
	private List<OnlineUserAns> onlineuserlist;
	private String session;
	private int sgid;
	
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
	public List<OnlineUserAns> getOnlineuserlist() {
		return onlineuserlist;
	}
	public void setOnlineuserlist(List<OnlineUserAns> onlineuserlist) {
		this.onlineuserlist = onlineuserlist;
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
}
