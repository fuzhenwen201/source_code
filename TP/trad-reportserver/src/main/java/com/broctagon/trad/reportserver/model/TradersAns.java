package com.broctagon.trad.reportserver.model;

import java.util.List;

public class TradersAns {

	/*bodyStr2={"reqid":"7699a6cd942448e1920f49d76ff2cc72","traderlist":"[
	{\"clientid\":1,\"groupid\":0,\"groupname\":\"Fox\",\"password\":\"2\",\"firstname\":\"wang\",\"lastname\":\"shangxiu\",\"email\":\"dsd@dm.com\",\
	"address\":\"baoan\",\"city\":\"ShenZhen\",\"state\":\"guangdong\",\"country\":\"Brazil\",\"pin\":\"13123123456\",\"accountid\":138,\"balance\":1200,\"prevbalance\":0,\
	"changedeposit\":1200,\"margin\":0,\"freemargin\":1200,\"closebalance\":0,\"boaccount\":0,\"fxaccount\":0,\"managerid\":0},
*/	
	
	private String reqid;
//	private List<Trader> traderlist;
	private String traderlist;
	private String session;
	
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
/*	public List<Trader> getTraderlist() {
		return traderlist;
	}
	public void setTraderlist(List<Trader> traderlist) {
		this.traderlist = traderlist;
	}*/
	public String getTraderlist() {
		return traderlist;
	}
	public void setTraderlist(String traderlist) {
		this.traderlist = traderlist;
	}
	public String getSession() {
		return session;
	}
	public void setSession(String session) {
		this.session = session;
	}
	
	
}