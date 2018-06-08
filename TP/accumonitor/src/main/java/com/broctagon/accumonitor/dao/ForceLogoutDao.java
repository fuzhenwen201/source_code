package com.broctagon.accumonitor.dao;

import java.util.List;

public interface ForceLogoutDao {

	public void addForceLogoutReq(String requestId, String message);

	public void addForceLogoutAns(String requestId, String message);
	
	public void addLoginInfo(String requestId, String loginReq, String loginAns);
	
	public List<String> getRequestIdByEmptyForceLoginReq();
	
}
