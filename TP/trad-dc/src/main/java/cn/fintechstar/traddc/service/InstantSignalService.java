package cn.fintechstar.traddc.service;

import java.util.EnumSet;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.data.redis.core.ListOperations;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import com.alibaba.fastjson.JSON;

import cn.fintechstar.traddc.model.Candle;
import cn.fintechstar.traddc.model.Granularity;
import cn.fintechstar.traddc.model.InstantSignal;
import cn.fintechstar.traddc.util.CandleUtil;
import cn.fintechstar.traddc.util.CommonUtil;
import cn.fintechstar.traddc.util.DateUtil;

@Service
public class InstantSignalService {
	
	private static Logger logger = Logger.getLogger(InstantSignalService.class);
	
	@Value("${candle.sameCandleTime}")
	private int sameCandleTimes;
	
	@Value("${candle.maxSize}")
	private int maxSize;
	
	@Autowired
	private StringRedisTemplate redisTemplate;
	
	public static final EnumSet<Granularity> granularitySet = EnumSet.allOf(Granularity.class);
	
	public void getHistoricalCandles(InstantSignal instantSignal){
		Map<Granularity, Candle> hisotoricalCandleMap = getTheLastestHistoricalCandles(instantSignal);
		if(!hisotoricalCandleMap.isEmpty()){
			calculateHistoricalCandles(hisotoricalCandleMap, instantSignal);
		}
	}

	private void calculateHistoricalCandles(Map<Granularity, Candle> hisotoricalCandleMap, InstantSignal instantSignal){
		for(Entry<Granularity, Candle> entry: hisotoricalCandleMap.entrySet()){
			if(!Granularity.NULL_VAL.equals(entry.getKey())){
				Candle lastestHistoricalCandle = entry.getValue();
				if(Granularity.S5.equals(entry.getKey())){
					if (sameInstantSignal(entry.getValue().getPreSignal(), instantSignal)) {
						int count = lastestHistoricalCandle.getCount() + 1;
						if(count == sameCandleTimes){
							logger.error("Received the same signal " + sameCandleTimes + " times");
						}else{
							lastestHistoricalCandle.setCount(count);
							lastestHistoricalCandle.setEndTime(instantSignal.getTimestamp());
							redisTemplate.opsForList().set(CommonUtil.getHistoryKey(instantSignal.getInstrument(), entry.getKey()), 0, JSON.toJSONString(lastestHistoricalCandle));
						}
					}
				}
				calculateHistoricalCandle(lastestHistoricalCandle, instantSignal, entry.getKey());
			}
		}
	}

	private Map<Granularity, Candle> getTheLastestHistoricalCandles(InstantSignal instantSignal){
		Map<Granularity, Candle> hisotoricalCandleMap = new HashMap<>();
		for(Granularity granularity : granularitySet){
			if(!Granularity.NULL_VAL.equals(granularity)){
				String key = CommonUtil.getHistoryKey(instantSignal.getInstrument(), granularity);
				String historicalCandleStr = redisTemplate.opsForList().index(key, 0);
				if(!StringUtils.isEmpty(historicalCandleStr)){
					hisotoricalCandleMap.put(granularity, JSON.parseObject(historicalCandleStr, Candle.class));
				}else{
					initHistoricalCandle(instantSignal, granularity, 2);
				}
			}
		}
		return hisotoricalCandleMap;
	}

	private void calculateHistoricalCandle(Candle historicalCandle, InstantSignal instantSignal, Granularity granularity){
		String key = CommonUtil.getHistoryKey(instantSignal.getInstrument(), granularity);
		long marketEndTime = DateUtil.getTimestamp(historicalCandle.getTime());
		long nextMarketStartTime = marketEndTime + 0000001;
		long marketStartTime = marketEndTime - DateUtil.getSecondsByGranularity(granularity, marketEndTime) * 1000000;
		long currentTime = instantSignal.getTimestamp();
		ListOperations<String, String> opsForList = redisTemplate.opsForList();
		if(currentTime >= marketStartTime && currentTime < nextMarketStartTime){
			getHistorialCandle(historicalCandle, instantSignal, currentTime);
			historicalCandle.setPreSignal(instantSignal);
			opsForList.set(key, 0, JSON.toJSONString(historicalCandle));
		}else if(currentTime < marketStartTime){
			System.err.println(DateUtil.getCurrentDateTimeStr(currentTime));
			System.err.println(DateUtil.getCurrentDateTimeStr(marketStartTime));
			System.err.println(DateUtil.getCurrentDateTimeStr(marketEndTime));
			String preHistoricalCandleStr = opsForList.index(key, 1);
			Candle preHistoricalCandle = JSON.parseObject(preHistoricalCandleStr, Candle.class);
			getHistorialCandle(preHistoricalCandle, instantSignal, currentTime);
			opsForList.set(key, 1, JSON.toJSONString(historicalCandle));
		}else{
			if(historicalCandle.getTime().equals(DateUtil.getMarketStartTimeByGranularity(currentTime, granularity))){
				if(historicalCandle.getDirty() == 2){
					historicalCandle.setComplete(false);
				}else{
					historicalCandle.setComplete(true);
				}
				historicalCandle.setDirty(0);
				opsForList.set(key, 0, JSON.toJSONString(historicalCandle));
				initHistoricalCandle(instantSignal, granularity, 0);
			}else{
				// if sp doesn't provide one data in 5 seconds, we put pre-historical candle to the database and regard this candle dirty
				historicalCandle.setDirty(1);
				historicalCandle.setComplete(true);
				int count = DateUtil.getDValueBetweenTwoTimestamp(historicalCandle.getEndTime(), currentTime, granularity);
			    for(int i = count -1; i > -1; i --){
			    	historicalCandle.setTime(getHistoricalCandleTime(currentTime, i, granularity));
			    	opsForList.leftPush(key, JSON.toJSONString(historicalCandle));
			    }
			    if(opsForList.size(key) > maxSize){
			    	opsForList.trim(key, 0, maxSize);
			    }
			}
		}
	}

	private void getHistorialCandle(Candle historicalCandle, InstantSignal instantSignal, long currentTime) {
		double ask = CommonUtil.intToDoubleByNumberFormat(instantSignal.getAsk());
		double bid = CommonUtil.intToDoubleByNumberFormat(instantSignal.getBid());
		if(ask > historicalCandle.getHighAsk()){
			historicalCandle.setHighAsk(ask);
			historicalCandle.setHighBid(bid);
		}
		if(bid < historicalCandle.getLowBid()){
			historicalCandle.setLowAsk(ask);
			historicalCandle.setLowBid(bid);
		}
		if(historicalCandle.getStartTime() > currentTime){
			historicalCandle.setOpenAsk(ask);
			historicalCandle.setOpenBid(bid);
			historicalCandle.setStartTime(currentTime);
		}
		if(historicalCandle.getEndTime() < currentTime){
			historicalCandle.setCloseAsk(ask);
			historicalCandle.setCloseBid(bid);
			historicalCandle.setEndTime(currentTime);
		}
	}

	private void initHistoricalCandle(InstantSignal instantSignal, Granularity granularity, int isFirstComeIn){
		Candle candle = new Candle();
		candle.setOpenAsk(instantSignal.getAsk());
		candle.setOpenBid(instantSignal.getBid());
		candle.setHighAsk(instantSignal.getAsk());
		candle.setHighBid(instantSignal.getBid());
		candle.setLowAsk(instantSignal.getAsk());
		candle.setLowBid(instantSignal.getBid());
		candle.setCloseAsk(instantSignal.getAsk());
		candle.setCloseBid(instantSignal.getBid());
		candle.setDirty(isFirstComeIn); // 1 :  dirty data, 2 : first come in 
		candle.setCount(1);
		candle.setComplete(false);
		candle.setStartTime(instantSignal.getTimestamp());
		candle.setEndTime(instantSignal.getTimestamp());
		candle.setPreSignal(instantSignal);
		candle.setTime(DateUtil.getMarketEndTimeStrByGranularity(instantSignal.getTimestamp(), granularity));
		redisTemplate.opsForList().leftPush(CommonUtil.getHistoryKey(instantSignal.getInstrument(), granularity), JSON.toJSONString(CandleUtil.formatCandle(candle)));
	}
	
	private boolean sameInstantSignal(InstantSignal preSignal, InstantSignal instantSignal) {
		return instantSignalToString(preSignal).equals(instantSignalToString(instantSignal));
	}
	
	public String instantSignalToString(InstantSignal instantSignal) {
		StringBuilder sb = new StringBuilder();
		sb.append("instrument : ").append(instantSignal.getInstrument()).append(", ")
		  .append("status : ").append(instantSignal.getStatus()).append(", ")
		  .append("ask : ").append(instantSignal.getAsk()).append(", ")
		  .append("bid : ").append(instantSignal.getBid()).append(".");
		return sb.toString();
	}
	
	public String getHistoricalCandleTime(long signalTime, int count, Granularity granularity){
		return DateUtil.getPreMarketEndTime(signalTime, count, granularity);
	}
}
