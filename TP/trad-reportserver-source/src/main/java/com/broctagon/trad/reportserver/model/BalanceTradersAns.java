package com.broctagon.trad.reportserver.model;

import java.util.List;

public class BalanceTradersAns {

/*	EGETTRADERSANS = 0x0078,
			{"reqid":"fcafb612f33e41359c3b2ef72533e1f8","traderlist":"[{\"clientid\":1,\"groupid\":0,\"groupname\":"
					+ "\"Fox\",\"password\":\"2\",\"firstname\":\"wang\",\"lastname\":\"shangxiu\",\"email\":\"dsd@dm.com\",\"address\":\"baoan\",\"city\":\"SZ\",\"state\":\"guangdong\",\"country\":\"Brazil\",\"pin\":\"13123123456\",\"accountid\":138,\"balance\":3000,\"prevbalance\":0,\"changedeposit\":3000,\"margin\":0,\"freemargin\":3000,\"closebalance\":0,\"boaccount\":0,\"fxaccount\":0,\"managerid\":0},{\"clientid\":20016,\"groupid\":0,\"groupname\":\"Fox\",\"password\":\"123\",\"firstname\":\"temp\",\"lastname\":\"temp\",\"email\":\"11113334@qq.com\",\"accountid\":219,\"balance\":0,\"prevbalance\":0,\"changedeposit\":0,\"margin\":0,\"freemargin\":"
					+ "0,\"closebalance\":0,\"boaccount\":0,\"fxaccount\":0,\"managerid\":0}]","iseod":1,
					"session":"ceaf6bf2634011e7aaf71b925476a83c","sgid":6,"managerid":1}*/
	
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
