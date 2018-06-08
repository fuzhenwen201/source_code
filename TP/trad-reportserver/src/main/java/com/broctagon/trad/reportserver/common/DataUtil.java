package com.broctagon.trad.reportserver.common;

import java.util.List;

import org.apache.log4j.Logger;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.reportserver.model.TradersAns;
import com.broctagon.trad.reportserver.model.Trader;

public class DataUtil {

	private static Logger logger = Logger.getLogger(DataUtil.class);
	
	public static List<Trader> getTraderList(byte[] body){
		if(body==null){
			return null;
		}
		byte[] decodeBody = body;
		String bodyStr = new String(decodeBody);
		TradersAns getTradersAns = JSON.parseObject(bodyStr, TradersAns.class);
		if(getTradersAns==null){
			logger.error("getTradersAns==null, bodyStr:"+bodyStr);
			return null;
		}
		
		String traderlistStr = getTradersAns.getTraderlist();
		List<Trader> traderList = JSON.parseArray(traderlistStr,Trader.class);
		return traderList;			
	}
	
/*	public static List<Order> getOrderList(){
		
	}*/
	
}
