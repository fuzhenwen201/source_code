package com.broctagon.accumonitor.dao;

import java.util.List;

public interface LoginDao {
	
	void addLoginReq(String requestId, String message);

	void addLoginAns(String requestId, String message);
	
	List<String> getLoginInfoByRequestId(String requestId);
	
	void deleteMessageByRequestId(String requestId);
}
