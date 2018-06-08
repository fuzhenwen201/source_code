package com.broctagon.trad.reportserver.serviceimpl;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.reportserver.common.CommonUtil;
import com.broctagon.trad.reportserver.common.Instrument;
import com.broctagon.trad.reportserver.dao.TradeDao;
import com.broctagon.trad.reportserver.model.Order;
import com.broctagon.trad.reportserver.model.Trade;
import com.broctagon.trad.reportserver.service.MT4TradeService;

@Service("MT4TradeService")
public class MT4TradeServiceImpl implements MT4TradeService{
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private TradeDao tradeDao;
	
	public void addMT4Trades(List<String> orderStrList){		
		List<Trade> tradeList = new ArrayList<Trade>();
		try{
			for(String orderStr: orderStrList){
				Order order = JSON.parseObject(orderStr, Order.class);
				if(order!=null){
					Trade trade = getTrade(order);
					tradeList.add(trade);	
				}
			}
				
			if(tradeList.size()==0){
				logger.info("tradeList.size()==0");
				return;
			}					
			long start = System.currentTimeMillis();
			try{
				tradeDao.addTrades(tradeList);
			}
			catch(Exception ex){
				ex.printStackTrace();
			}				
			long end = System.currentTimeMillis();
			logger.info("tradeList.size():" + tradeList.size() + ", interval:" + (end-start));
		}
		catch(Exception e){
			logger.error(e.getMessage());
		}
				
	}
		
/*	public List<Trade> getOrderList(List<String> orderStrList){
		List<Trade> tradeList = new ArrayList<Trade>();
		for(String orderStr: orderStrList){
			Order order = JSON.parseObject(orderStr, Order.class);
			Trade trade = getTrade(order);
			tradeList.add(trade);
		}
		return tradeList;
	}*/

	public Trade getTrade(Order order){
		
		long currentTime = System.currentTimeMillis(); 
		
		Trade trade = new Trade();
		trade.setVolume(order.getAmount());
//		trade.setTicket(ticket);
		trade.setOrderid(order.getOrderid());
		trade.setLogin(order.getClientid());
		
		String symbol = "";
		try{
			Instrument instrument = Instrument.get((short)order.getAssetid());
			if(instrument!=null){
				symbol = instrument.toString();
			}
		}
		catch(Exception ex){
			logger.error(ex.getMessage());
		}		
		trade.setSymbol(symbol);
		
		trade.setDigits(6);
		trade.setCmd(order.getType());
		
		long openTime= order.getOpentime();
		if(openTime>currentTime*10){
	//		logger.info("openTime>currentTime*10, open time:"+ openTime + ",currentTime:" + currentTime);
			openTime=openTime/1000;
		}		
		trade.setOpen_time(new Timestamp(openTime));
		trade.setOpen_price(order.getOpenprice()/1000000.0);
		trade.setSl(0.1);
		trade.setTp(0.1);
		long closeTime = openTime+order.getExpiryperiod()*1000;
		trade.setClose_time(new Timestamp(closeTime));
		long expiryTime = order.getExpirytime();	
		if(expiryTime>currentTime*10){
	//		logger.info("openTime>currentTime*10, open time:"+ openTime + ",currentTime:" + currentTime);
			expiryTime=expiryTime/1000;
		}	
		trade.setExpiration(new Timestamp(expiryTime));
		trade.setReason(1);
		trade.setConv_rate1(0.1);
		trade.setConv_rate2(0.1);
		trade.setCommission(0.1);
		trade.setCommission_agent(0.1);
		trade.setSwaps(0.1);
		trade.setClose_price(order.getCloseprice()/1000000.0);
		trade.setProfit(0.1);
		trade.setTaxes(0.1);
		String orderId = order.getOrderid();
		if(orderId.length()>32){
			orderId=orderId.substring(0, 32);
		}
		
		if(!CommonUtil.isEmptyString(order.getComment())){
			trade.setComment(order.getComment());
		}
		else{
			trade.setComment(orderId);
		}
		
		trade.setInternal_id(1);
		trade.setMargin_rate(0.1);
		trade.setTimestamp(1);
		trade.setMagic(1);
		trade.setGw_volume(1);
		trade.setGw_open_price(order.getOpenprice());
		trade.setGw_close_price(order.getCloseprice());
		if(order.getCloseprice()<=0){
			trade.setModify_time(new Timestamp(openTime));
		}
		else{
			trade.setModify_time(new Timestamp(closeTime));
		}
		
		return trade;
	}
	
}
