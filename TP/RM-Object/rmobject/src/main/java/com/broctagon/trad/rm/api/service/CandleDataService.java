/**
 * 
 */
package com.broctagon.trad.rm.api.service;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.UUID;

import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.api.model.Bar;
import com.broctagon.trad.rm.api.model.CandleDataReq;
import com.broctagon.trad.rm.api.model.CandleDataRes;



/**
 * @author imdadullah
 *
 */
@Service
public class CandleDataService {

	public CandleDataRes getCandleData(String message){
		List<Bar> candleList = new ArrayList<>();
		CandleDataRes dataRes = new CandleDataRes();
		CandleDataReq req = JSON.parseObject(message, CandleDataReq.class);
		int barCount = req.getCandlecount();
		String instrument = req.getSymbol();
		String granularity = req.getGranularity();
		
		for(int i=1;i<=barCount;i++){
			Bar candle = new Bar();
			if(i==1){
				candle.setComplete(false);
			}
			else{
				candle.setComplete(true);
			}
			
			double num = 0.00005;
			double baseBid = 1.90471;
			double baseClose = 1.90500;
			double baseOpen = 1.90482;
			double baseAsk = 1.90482;
			
			if (i%2==0){
				num = num*(-1);
			}
			candle.setCloseAsk(baseClose+num);
			candle.setCloseBid(baseClose+num);
			candle.setCount(barCount);
			candle.setHighAsk(baseAsk+num);
			candle.setHighBid(baseBid+num);
			candle.setLowAsk(baseAsk+num);
			candle.setLowBid(baseBid+num);
			candle.setOpenAsk(baseOpen+num);
			candle.setOpenBid(baseOpen+num);
			candle.setTime((System.currentTimeMillis()-5000*i)*1000);
			
			candleList.add(candle);
		}
		dataRes.setInstrument(instrument);
		dataRes.setGranularity(granularity);
		dataRes.setBars(candleList);
		//return JSON.toJSONString(dataRes);
		return dataRes;
	}
	
	
	public String getRandomeNumber(){
		UUID uuid = UUID.randomUUID();
    	String reqid = uuid.toString().replace("-", "");
    	
		return reqid;
		
	}
	
	public static String getDateTimeStringFormat(){
		Date date = new Date(System.currentTimeMillis());
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		int year = calendar.get(Calendar.YEAR);
		int month = calendar.get(Calendar.MONTH) + 1;
		int day = calendar.get(Calendar.DATE);
		int hour = calendar.get(Calendar.HOUR_OF_DAY);
		int minute = calendar.get(Calendar.MINUTE);
		int second = calendar.get(Calendar.SECOND);
		return new StringBuilder().append(day+"_").append(month+"_").append(year+"_").append(hour+"_").append(minute+"_").append(second).toString();
	}



}
