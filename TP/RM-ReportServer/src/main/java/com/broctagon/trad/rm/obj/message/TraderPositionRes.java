package com.broctagon.trad.rm.obj.message;

import java.util.List;

import com.broctagon.trad.rm.db.model.Order;



;
/**
 * @author imdadullah
 *
 */
public class TraderPositionRes {

	private int clientid;
	private String reqid;
	private int sgid;
	private int managerid;
	private String session;
	private int total;
	private List<Order> body;
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
	public int getClientid() {
		return clientid;
	}
	public void setClientid(int clientid) {
		this.clientid = clientid;
	}
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public int getSgid() {
		return sgid;
	}
	public void setSgid(int sgid) {
		this.sgid = sgid;
	}
	public String getSession() {
		return session;
	}
	public void setSession(String session) {
		this.session = session;
	}
	public int getTotal() {
		return total;
	}
	public void setTotal(int total) {
		this.total = total;
	}
	public List<Order> getBody() {
		return body;
	}
	public void setBody(List<Order> body) {
		this.body = body;
	}
	
}
