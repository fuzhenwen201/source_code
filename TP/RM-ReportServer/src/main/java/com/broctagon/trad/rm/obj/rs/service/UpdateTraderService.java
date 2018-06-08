package com.broctagon.trad.rm.obj.rs.service;

import java.util.List;

import com.broctagon.trad.rm.obj.model.BalanceTrader;

public interface UpdateTraderService {

	public void broadcastUpdateTrader();
	public void publishOnlineTraderBalance();
	
	public List<BalanceTrader> fetchOnlineBalanceTrader();
}
