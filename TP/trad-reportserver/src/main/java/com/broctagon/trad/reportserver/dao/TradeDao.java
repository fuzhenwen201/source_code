package com.broctagon.trad.reportserver.dao;

import java.util.List;

import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.broctagon.trad.reportserver.model.Trade;

@Repository
@Transactional
public interface TradeDao {

	public Trade getTradeByTicketId(Integer ticketId);
	public List<Trade> getTradesByTicketIds(List<Integer> ticketIds);
	
	public Trade getTradeByOrderId(String orderId);
	public int getTicketByOrderId(String orderId);
	
	public int addTrade(Trade trade);
	public int addTrades(List<Trade> trades);
	
	public int updateTrade(Trade trade);
	public int updateTrades(List<Trade> trades);

	public int deleteTradeByTicketId(Integer ticketId);
	public int deleteTradesByTicketIds(List<Integer> ticketIds);
	
}
