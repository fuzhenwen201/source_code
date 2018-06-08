package com.broctagon.trad.reportserver.serviceimpl;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.reportserver.dao.PriceDao;
import com.broctagon.trad.reportserver.model.Price;
import com.broctagon.trad.reportserver.model.RealtimeData;
import com.broctagon.trad.reportserver.service.MT4PriceService;
import com.broctagon.trad.reportserver.util.DateUtil;

@Service("priceService")
public class MT4PriceServiceImpl implements MT4PriceService{

	@Autowired
	private PriceDao priceDao;
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	public void updateMT4Price(){
		
	}
	
	public void addMT4Prices(List<Object> realtimelist){		
		List<Price> priceList = getPriceList(realtimelist);
		if(priceList==null||priceList.size()==0){
			logger.error("(priceList==null||priceList.size()==0");
			return;
		}
		priceDao.addPrices(priceList);				
	}
	
//	{"ask":1057890,"bid":1057580,"instrument":"AUD_NZD","status":1,"timestamp":1499907679544500}	
	public List<Price> getPriceList(List<Object> realTimeDataList){			
	//	logger.info("DateUtil.getTimestamp(new Date()):" + DateUtil.getTimestamp(new Date()) );				
		List<Price> priceList = new ArrayList<Price>();
		for(Object realtimeStr: realTimeDataList){	
		//	logger.debug(realtimeStr.toString());
			if(realtimeStr!=null){
				RealtimeData realtimeData = JSON.parseObject(realtimeStr.toString(), RealtimeData.class);
				Price price = new Price();
				price.setSymbol(realtimeData.getInstrument().toString());
				long currentTime = System.currentTimeMillis();
				long time = realtimeData.getTimestamp().longValue();
				if(time>currentTime*10){
					time=time/1000;
				}
				price.setTime(new Timestamp(time));
				price.setBid(realtimeData.getBid()/1000000.0);	
				price.setAsk(realtimeData.getAsk()/1000000.0);
				price.setLow(0);		
				price.setHigh(0);
				price.setDirection(0);
				price.setDigits(5);
				price.setSpread(0);
				price.setModify_time(new Timestamp(time));
				
			//	logger.info("price:"+JSON.toJSONString(price));	
				priceList.add(price);
			}
		}
		return priceList;		
	}
	
}
