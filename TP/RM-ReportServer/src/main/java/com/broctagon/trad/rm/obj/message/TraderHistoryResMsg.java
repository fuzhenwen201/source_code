/**
 * 
 */
package com.broctagon.trad.rm.obj.message;

import java.util.List;

import com.broctagon.trad.rm.db.model.Order;




/**
 * @author imdadullah
 *
 */
public class TraderHistoryResMsg {
	private int clientid;
	private String reqid;
	private int groupid;
	private int sgid;
	private int managerid;
	private String sessionid;
	private int total;
	private List<Order> body;
	public String getSessionid() {
		return sessionid;
	}
	public void setSessionid(String sessionid) {
		this.sessionid = sessionid;
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
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
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
