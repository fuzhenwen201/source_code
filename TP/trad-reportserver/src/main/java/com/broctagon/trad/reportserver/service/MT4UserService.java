package com.broctagon.trad.reportserver.service;

import java.util.List;

public interface MT4UserService {
	
	public void addMT4Users(List<String> traderlist);
	public void updateBalanceUsers(List<String> traderList);
		
}
