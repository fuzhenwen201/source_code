package com.broctagon.trad.reportserver.dao.perf;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.trad.reportserver.config.DruidConfig;
import com.broctagon.trad.reportserver.dao.TradeDao;
import com.broctagon.trad.reportserver.model.Price;
import com.broctagon.trad.reportserver.model.Trade;
import com.broctagon.trad.reportserver.util.DateUtil;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = {DruidConfig.class})
public class TradeDaoPerfTest {

	@Autowired
	private TradeDao dao;
	
	private List<Trade> trades;
	
	private Trade trade;
	
	private List<Integer> ids;
	
	@Before
	public void init(){
		trades = new ArrayList<>();
		ids = new ArrayList<>();
		for(int i = 1; i < 10001; i ++){
			Trade trade = new Trade();
			trade.setTicket(i);
			trade.setLogin(i);
			trade.setSymbol("test");
			trade.setDigits(i);
			trade.setCmd(i);
			trade.setOpen_time(DateUtil.getTimestamp(new Date()));
			trade.setOpen_price(i);
			trade.setSl(i);
			trade.setTp(i);
			trade.setClose_time(DateUtil.getTimestamp(new Date()));
			trade.setExpiration(DateUtil.getTimestamp(new Date()));
			trade.setReason(i);
			trade.setConv_rate1(i);
			trade.setConv_rate2(i);
			trade.setCommission(i);
			trade.setCommission_agent(i);
			trade.setSwaps(i);
			trade.setClose_price(i);
			trade.setProfit(i);
			trade.setTaxes(i);
			trade.setComment("test");
			trade.setInternal_id(i);
			trade.setMargin_rate(i);
			trade.setTimestamp(i);
			trade.setMagic(i);
			trade.setGw_volume(i);
			trade.setGw_open_price(i);
			trade.setGw_close_price(i);
			trade.setModify_time(DateUtil.getTimestamp(new Date()));
			trades.add(trade);
			ids.add(trade.getTicket());
		}
		
		trade = new Trade();
		trade.setTicket(-1);
		trade.setLogin(-1);
		trade.setSymbol("test");
		trade.setDigits(-1);
		trade.setCmd(-1);
		trade.setOpen_time(DateUtil.getTimestamp(new Date()));
		trade.setOpen_price(0.1);
		trade.setSl(0.1);
		trade.setTp(0.1);
		trade.setClose_time(DateUtil.getTimestamp(new Date()));
		trade.setExpiration(DateUtil.getTimestamp(new Date()));
		trade.setReason(-1);
		trade.setConv_rate1(0.1);
		trade.setConv_rate2(0.1);
		trade.setCommission(0.1);
		trade.setCommission_agent(0.1);
		trade.setSwaps(0.1);
		trade.setClose_price(0.1);
		trade.setProfit(0.1);
		trade.setTaxes(0.1);
		trade.setComment("test");
		trade.setInternal_id(-1);
		trade.setMargin_rate(0.1);
		trade.setTimestamp(1);
		trade.setMagic(-1);
		trade.setGw_volume(-1);
		trade.setGw_open_price(-1);
		trade.setGw_close_price(-1);
		trade.setModify_time(DateUtil.getTimestamp(new Date()));
	}
	
//	@Test
	public void testAddOne(){
		long startTime = System.currentTimeMillis();
		dao.addTrade(trade);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testAddAll(){
		List<Trade> trades1 = trades.subList(0, 2500);
		List<Trade> trades2 = trades.subList(2501, 5000);
		List<Trade> trades3 = trades.subList(5001, 7500);
		List<Trade> trades4 = trades.subList(7501, 9999);
		long startTime = System.currentTimeMillis();
		dao.addTrades(trades1);
		dao.addTrades(trades2);
		dao.addTrades(trades3);
		dao.addTrades(trades4);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		// TODO -- It seems only can support 1w in 2s,maybe need to change jdbc template to update  
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testUpdateOne(){
		trade.setLogin(2);
		long startTime = System.currentTimeMillis();
		dao.updateTrade(trade);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testUpdateAll(){
		for(Trade trade : trades){
			trade.setTaxes(0.0);
			trade.setClose_price(0.0);
		}
		List<Trade> trades1 = trades.subList(0, 2500);
		List<Trade> trades2 = trades.subList(2501, 5000);
		List<Trade> trades3 = trades.subList(5001, 7500);
		List<Trade> trades4 = trades.subList(7501, 9999);
		long startTime = System.currentTimeMillis();
		dao.updateTrades(trades1);
		dao.updateTrades(trades2);
		dao.updateTrades(trades3);
		dao.updateTrades(trades4);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		// TODO -- It seems only can support 1w in 2s,maybe need to change jdbc template to update  
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testSelectOne(){
		long startTime = System.currentTimeMillis();
		Trade trade = dao.getTradeByTicketId(-1);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testSelectAll(){
		long startTime = System.currentTimeMillis();
		List<Trade> trades = dao.getTradesByTicketIds(ids);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testDeleteOne(){
		long startTime = System.currentTimeMillis();
		dao.deleteTradeByTicketId(-1);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testDeleteAll(){
		long startTime = System.currentTimeMillis();
		dao.deleteTradesByTicketIds(ids);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
}
