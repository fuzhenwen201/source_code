package com.broctagon.trad.rm.db.dao;

import java.util.List;

import com.broctagon.trad.rm.db.model.TradeModel;

public interface OpenTradeDao {
	
	public List<TradeModel> selectOpenTradeByClientid(int clientid);
	public List<TradeModel> selectAllOpenTrade();
	
	public int insertOpenTrade(TradeModel openTrade);
	public int insertOpenTrades(List<TradeModel> openTrades);
	
	public void deleteOpenTrade(String orderid);
	public void deleteOpenTrades(List<String> orderids);

}
