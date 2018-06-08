package com.broctagon.accumonitor.dao;

public interface LogoutDao {
	
	void addLogoutReq(String requestId, String message);

	void addLogoutAns(String requestId, String message);
	
	void detelteLogoutByRequestId(String requestId);
}
