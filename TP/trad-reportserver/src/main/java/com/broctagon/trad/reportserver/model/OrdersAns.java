package com.broctagon.trad.reportserver.model;

import java.util.List;

public class OrdersAns {
	
/*	{"clientid":0,"reqid":"0cc458dac629438ba7d64cc28c55c3e1","sgid":6,"managerid":1,"session":"ceaf6bf2634011e7aaf71b925476a83c","total":14,"body":
		[{"reqid":"4e978baf75d54a5d8bca9c0491f9ff9d","groupid":1,"clientid":82,"orderid":"22922d0e631e11e7aaf7b64ac1597823","orderstatus":0,"assetid":1,"direction":0,"amount":100,
			"expiryperiod":0,"opentime":1499436673826456,"openprice":729000,"closeprice":0,"payout":0,"sgid":1,"teid":0,"expirytime":"1499436673871456"},
		 {"reqid":"cc62688b6d624e6ab20282af17cb1281","groupid":1,"clientid":82,"orderid":"2340e268631e11e7aaf783a9b216c574","orderstatus":0,"assetid":1,"direction":1,"amount":100,
				"expiryperiod":0,"opentime":1499436674971404,"openprice":728960,"closeprice":0,"payout":0,"sgid":1,"teid":0,"expirytime":"1499436675016404"},
		 {"reqid":"7f1b5edb8e014826b152f47257cebea7","groupid":1,"clientid":82,"orderid":"2804a910631e11e7aaf7ac238469b517","orderstatus":0,"assetid":4,"direction":0,"amount":100,
					"expiryperiod":0,"opentime":1499436682965326,"openprice":1042650,"closeprice":0,"payout":0,"sgid":1,"teid":0,"expirytime":"1499436682970326"}]}*/

	private int clientid;
	private String reqid;
	private int sgid;
	private int managerid;
	private String session;
	private int total;
	private List<Order> body;
	
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
