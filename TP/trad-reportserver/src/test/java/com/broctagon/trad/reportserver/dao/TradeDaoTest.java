package com.broctagon.trad.reportserver.dao;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.trad.reportserver.config.DruidConfig;
import com.broctagon.trad.reportserver.model.Trade;
import com.broctagon.trad.reportserver.util.DateUtil;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = {DruidConfig.class})
public class TradeDaoTest {

	@Autowired	
	private TradeDao tradeDao;
	
//	@Test
	public void testGetTradeByTicketId(){
		Trade trade = tradeDao.getTradeByTicketId(93701631);
		System.out.println(trade);
	}
	
//	@Test
	public void testGetTradesByTicketIds(){
		List<Integer> ticketIds = new ArrayList<>();
		ticketIds.add(93698604);
		ticketIds.add(93698605);
		ticketIds.add(93698606);
		List<Trade> trades = tradeDao.getTradesByTicketIds(ticketIds);
		System.out.println(trades);
	}
	
//	@Test
	public void testAddTrade(){
		Trade trade = new Trade();
		trade.setTicket(1425147);
		trade.setLogin(441);
		trade.setSymbol("test");
		trade.setClose_time(DateUtil.getTimestamp(new Date()));
		trade.setOpen_time(DateUtil.getTimestamp(new Date()));
		trade.setExpiration(DateUtil.getTimestamp(new Date()));
		trade.setComment("1");
		trade.setInternal_id(1);
		trade.setTimestamp(2222222);
		trade.setModify_time(DateUtil.getTimestamp(new Date()));
		int statusCode = tradeDao.addTrade(trade);
		System.out.println(statusCode);
	}
	
//	@Test
	public void testAddTrades(){
		List<Trade> trades = new ArrayList<>();
		Trade t1 = new Trade();
		t1.setTicket(12345);
		t1.setLogin(441);
		t1.setSymbol("symbol");
		t1.setClose_time(DateUtil.getTimestamp(new Date()));
		t1.setOpen_time(DateUtil.getTimestamp(new Date()));
		t1.setExpiration(DateUtil.getTimestamp(new Date()));
		t1.setComment("comment");
		t1.setInternal_id(1);
		t1.setTimestamp(2222222);
		t1.setModify_time(DateUtil.getTimestamp(new Date()));
		Trade t2 = new Trade();
		t2.setTicket(12345678);
		t2.setLogin(441);
		t2.setSymbol("update");
		t2.setClose_time(DateUtil.getTimestamp(new Date()));
		t2.setOpen_time(DateUtil.getTimestamp(new Date()));
		t2.setExpiration(DateUtil.getTimestamp(new Date()));
		t2.setComment("update");
		t2.setInternal_id(1);
		t2.setTimestamp(2222222);
		t2.setModify_time(DateUtil.getTimestamp(new Date()));
		trades.add(t1);
		trades.add(t2);
		tradeDao.addTrades(trades);
	}
	
//	@Test
	public void testUpdateTrade(){
		Trade trade = new Trade();
		trade.setTicket(21444);
		trade.setLogin(445);
		trade.setSymbol("n");
		trade.setClose_time(DateUtil.getTimestamp(new Date()));
		trade.setOpen_time(DateUtil.getTimestamp(new Date()));
		trade.setExpiration(DateUtil.getTimestamp(new Date()));
		trade.setComment("1");
		trade.setInternal_id(1);
		trade.setTimestamp(2222222);
		trade.setModify_time(DateUtil.getTimestamp(new Date()));
		int statusCode = tradeDao.updateTrade(trade);
		System.out.println(statusCode);
	}
	
	@Test
	public void testUpdateTrades(){
		List<Trade> trades = new ArrayList<>();
		Trade t1 = new Trade();
		t1.setLogin(4441);
		t1.setSymbol("tedddst");
		t1.setClose_time(DateUtil.getTimestamp(new Date()));
		t1.setOpen_time(DateUtil.getTimestamp(new Date()));
		t1.setExpiration(DateUtil.getTimestamp(new Date()));
		t1.setComment("1");
		t1.setInternal_id(1);
		t1.setTimestamp(2222222);
		t1.setModify_time(DateUtil.getTimestamp(new Date()));
		t1.setTicket(93698602);
		Trade t2 = new Trade();
		t2.setLogin(4442);
		t2.setSymbol("tesddt");
		t2.setClose_time(DateUtil.getTimestamp(new Date()));
		t2.setOpen_time(DateUtil.getTimestamp(new Date()));
		t2.setExpiration(DateUtil.getTimestamp(new Date()));
		t2.setComment("1");
		t2.setInternal_id(1);
		t2.setTimestamp(2222222);
		t2.setModify_time(DateUtil.getTimestamp(new Date()));
		t2.setTicket(93698606);
		Trade t3 = new Trade();
		t3.setLogin(4443);
		t3.setSymbol("test");
		t3.setClose_time(DateUtil.getTimestamp(new Date()));
		t3.setOpen_time(DateUtil.getTimestamp(new Date()));
		t3.setExpiration(DateUtil.getTimestamp(new Date()));
		t3.setComment("1");
		t3.setInternal_id(1);
		t3.setTimestamp(2222222);
		t3.setModify_time(DateUtil.getTimestamp(new Date()));
		t3.setTicket(93698607);
		trades.add(t1);
		trades.add(t2);
		trades.add(t3);
		int statusCode = tradeDao.updateTrades(trades);
		System.out.println(statusCode);
	}
	
//	@Test
	public void testDeleteTradeByTicketId(){
		int statusCode = tradeDao.deleteTradeByTicketId(55);
		System.out.println(statusCode);
	}
	
//	@Test
	public void testDeleteTradesByTicketIds(){
		List<Integer> ids = new ArrayList<>();
		ids.add(93698607);
		ids.add(93717449);
		tradeDao.deleteTradesByTicketIds(ids);
	}
	
}
