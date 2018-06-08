/**
 * 
 */
package com.broctagon.trad.rm.obj.model;

import java.util.List;

import com.broctagon.trad.rm.obj.message.UserInfoMsg;

/**
 * @author imdadullah
 *
 */
public class UserInfoResModel {
  private List<UserInfoMsg> tradersaccountlist;

public List<UserInfoMsg> getTradersaccountlist() {
	return tradersaccountlist;
}

public void setTradersaccountlist(List<UserInfoMsg> tradersaccountlist) {
	this.tradersaccountlist = tradersaccountlist;
}
}
