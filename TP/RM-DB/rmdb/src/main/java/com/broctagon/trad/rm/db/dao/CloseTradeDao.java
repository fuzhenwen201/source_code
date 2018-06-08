package com.broctagon.trad.rm.db.dao;

import java.util.List;

import com.broctagon.trad.rm.db.model.TradeModel;

public interface CloseTradeDao {
	
	public int insertClosePosition(TradeModel clsoeTrade);
	public int insertClosePositions(List<TradeModel> clsoeTrades);
	
	public void deleteCloseTrade(String orderid);
	public void deleteCloseTrades(List<String> orderids);
	public void deleteAllCloseTrades();
	
	public List<TradeModel> selectCloseTradesByClientid(Integer clientid);
	public List<TradeModel> selectCloseTradesByTime();
	public List<TradeModel> selectAllCloseTrades();
}
