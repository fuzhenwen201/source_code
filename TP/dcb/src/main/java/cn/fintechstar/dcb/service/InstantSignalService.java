package cn.fintechstar.dcb.service;

import java.time.DayOfWeek;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.dao.DataAccessException;
import org.springframework.data.redis.connection.RedisConnection;
import org.springframework.data.redis.connection.StringRedisConnection;
import org.springframework.data.redis.core.RedisCallback;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import com.alibaba.fastjson.JSON;

import cn.fintechstar.dcb.config.HistoricalCandleConstant;
import cn.fintechstar.dcb.constant.GranularityConstant;
import cn.fintechstar.dcb.dao.InstantSignalDao;
import cn.fintechstar.dcb.model.Candle;
import cn.fintechstar.dcb.model.InstantSignal;
import cn.fintechstar.dcb.util.CandleUtil;
import cn.fintechstar.dcb.util.CommonUtil;
import cn.fintechstar.dcb.util.DateUtil;

@Service
public class InstantSignalService {
	
	private static Logger logger = Logger.getLogger(InstantSignalService.class);

	@Value("${candle.sameCandleTime}")
	private int sameCandleTimes;
	
	@Value("${candle.maxSize}")
	private int maxSize;
	
	@Autowired
	private StringRedisTemplate redisTemplate;
	
	@Autowired
	private InstantSignalDao instantSignalDao;
	
	public String getSaveInstantSignalSql(InstantSignal instantSignal) {
		StringBuilder sb = new StringBuilder();
		sb.append("INSERT INTO RAW_")
		  .append(instantSignal.getInstrument())
		  .append("(STATUS, BID, ASK, 'TIMESTAMP', TIME_FORMAT) VALUES( ")
		  .append("\"").append(instantSignal.getInstrument()).append("\",")
		  .append(CommonUtil.intToDoubleByNumberFormat(instantSignal.getBid())).append(",")
		  .append(CommonUtil.intToDoubleByNumberFormat(instantSignal.getAsk())).append(",")
		  .append(instantSignal.getTimestamp()).append(",")
		  .append("\"").append(DateUtil.getCurrentDateTimeStr(instantSignal.getTimestamp())).append("\");");
		return sb.toString();
	}
	
	public void saveInstantSignal(String sql) {
		instantSignalDao.saveInstantSignal(sql);	
	}
	
	@SuppressWarnings("rawtypes")
	public void getHistoricalCandles(InstantSignal instantSignal){
		Map<String, Candle> hisotoricalCandleMap = getTheLastestHistoricalCandles(instantSignal);
		
		if(!hisotoricalCandleMap.isEmpty()){
			List<String> redisOperations = getRedisOperations(hisotoricalCandleMap, instantSignal);
			redisTemplate.executePipelined(new RedisCallback() {
				@Override
				public Object doInRedis(RedisConnection connection) throws DataAccessException {
					StringRedisConnection redisConnection = (StringRedisConnection) connection;
					for(String redisOperation : redisOperations){
						String[] details = redisOperation.split(HistoricalCandleConstant.SPLITTER);
						String key = details[0];
						String operation = details[1];
						if(HistoricalCandleConstant.UPDATE_COUNT.equals(operation) || HistoricalCandleConstant.UPDATE_LATEST_INDEX.equals(operation)){
							redisConnection.lSet(key, 0, details[2]);
						}else if(HistoricalCandleConstant.UPDATE_PRE_INDEX.equals(operation)){
							redisConnection.lSet(key, 1, details[2]);
						}else if(HistoricalCandleConstant.INSERT_INDEX.equals(operation)){
							redisConnection.lPush(key, details[2]);
						}else{
							for(int i = 2; i < details.length; i ++){
								if(!StringUtils.isEmpty(details[i])){
									redisConnection.lPush(key, details[i]);
								}
							}
						}
					}
					return null;
				}
			});
		}
	}

	private Map<String, Candle> getTheLastestHistoricalCandles(InstantSignal instantSignal){
		Map<String, Candle> hisotoricalCandleMap = new HashMap<>();
		Set<String> initGranularity = new HashSet<>();
		initGranularity.addAll(GranularityConstant.getGranularityToSecondsMap().keySet());
		Set<String> granularityStoredSet = new HashSet<>();
		List<Object> historicalCandleStrList = redisTemplate.executePipelined(new RedisCallback<String>() {
			@Override
			public String doInRedis(RedisConnection connection) throws DataAccessException {
				StringRedisConnection redisConnection = (StringRedisConnection) connection;
				for(String granularity : GranularityConstant.getGranularityToSecondsMap().keySet()){
					String key = CommonUtil.getHistoryKey(instantSignal.getInstrument(), granularity);
					redisConnection.lIndex(key, 0);
				}
				return null;
			}
		});
		for(Object historicalCandleStr : historicalCandleStrList){
			if(historicalCandleStr != null){
				Candle candle = JSON.parseObject((String)historicalCandleStr, Candle.class);
				hisotoricalCandleMap.put(candle.getGranularity(), candle);
				granularityStoredSet.add(candle.getGranularity());
			}
		}
		initGranularity.removeAll(granularityStoredSet);
		if(initGranularity.size() > 0){
			redisTemplate.executePipelined(new RedisCallback<String>() {
				@Override
				public String doInRedis(RedisConnection connection) throws DataAccessException {
					StringRedisConnection redisConnection = (StringRedisConnection) connection;
					for(String granularity : initGranularity){
						Candle candle = initHistoricalCandle(instantSignal, granularity, 2);
						redisConnection.lPush(CommonUtil.getHistoryKey(instantSignal.getInstrument(), granularity), JSON.toJSONString(candle));
					}
					return null;
				}
			});
		}
		redisTemplate.executePipelined(new RedisCallback<String>() {
			@Override
			public String doInRedis(RedisConnection connection) throws DataAccessException {
				StringRedisConnection redisConnection = (StringRedisConnection) connection;
				for(String granularity : GranularityConstant.getGranularityToSecondsMap().keySet()){
					String key = CommonUtil.getHistoryKey(instantSignal.getInstrument(), granularity);
					redisConnection.lTrim(key, 0, maxSize);
				}
				return null;
			}
		});
		return hisotoricalCandleMap;
	}

	private List<String> getRedisOperations(Map<String, Candle> hisotoricalCandleMap, InstantSignal instantSignal) {
		List<String> redisOperations = new ArrayList<>();
		for(Entry<String, Candle> entry: hisotoricalCandleMap.entrySet()){
			Candle lastestHistoricalCandle = entry.getValue();
			InstantSignal preCandle = entry.getValue().getPreSignal();
			if(!preCandle.toString().equals(instantSignal.toString())){
				if("S5".equals(entry.getKey())){
					if (sameInstantSignal(preCandle, instantSignal)) {
						int count = lastestHistoricalCandle.getCount() + 1;
						if(count == sameCandleTimes){
							logger.error("Received the same signal " + sameCandleTimes + " times");
						}else{
							String key = CommonUtil.getHistoryKey(instantSignal.getInstrument(), entry.getKey()) + HistoricalCandleConstant.SPLITTER + HistoricalCandleConstant.UPDATE_COUNT;
							lastestHistoricalCandle.setCount(count);
							lastestHistoricalCandle.setEndTime(instantSignal.getTimestamp());
							String operation = key + HistoricalCandleConstant.SPLITTER + JSON.toJSONString(lastestHistoricalCandle); 
							redisOperations.add(operation);
						}
					}
				}
				List<String> updateHistoricalOperations = calculateHistoricalCandle(lastestHistoricalCandle, instantSignal, entry.getKey());
				redisOperations.addAll(updateHistoricalOperations);
			}
		}
		return redisOperations;
	}
	
	private List<String> calculateHistoricalCandle(Candle historicalCandle, InstantSignal instantSignal, String granularity){
		List<String> redisOperations = new ArrayList<>();
		String key = CommonUtil.getHistoryKey(instantSignal.getInstrument(), granularity) + HistoricalCandleConstant.SPLITTER;
		long marketEndTime = DateUtil.getTimestamp(historicalCandle.getTime());
		long nextMarketStartTime = marketEndTime + 0000001;
		long marketStartTime = marketEndTime - DateUtil.getSecondsByGranularity(granularity, marketEndTime) * 1000000;
		long currentTime = instantSignal.getTimestamp();
		if(currentTime >= marketStartTime && currentTime < nextMarketStartTime){
			getHistorialCandle(historicalCandle, instantSignal, currentTime);
			historicalCandle.setPreSignal(instantSignal);
			String operation = key + HistoricalCandleConstant.UPDATE_LATEST_INDEX + HistoricalCandleConstant.SPLITTER + JSON.toJSONString(historicalCandle);
			redisOperations.add(operation);
		}else if(currentTime < marketStartTime){
			String operation = key + HistoricalCandleConstant.UPDATE_PRE_INDEX + HistoricalCandleConstant.SPLITTER + JSON.toJSONString(historicalCandle);
			redisOperations.add(operation);
		}else{
			if(historicalCandle.getDirty() == 2){
				historicalCandle.setComplete(false);
			}else{
				historicalCandle.setComplete(true);
			}
			historicalCandle.setDirty(0);
			String updateOperation = key + HistoricalCandleConstant.UPDATE_LATEST_INDEX + HistoricalCandleConstant.SPLITTER + JSON.toJSONString(historicalCandle);
			redisOperations.add(updateOperation);
			if(!historicalCandle.getTime().equals(DateUtil.getMarketStartTimeStrByGranularity(currentTime, granularity))){
				// if sp doesn't provide one data in 5 seconds, we put pre-historical candle to the database and regard this candle dirty
				historicalCandle.setDirty(1);
				historicalCandle.setComplete(true);
				DayOfWeek preSignalDayOfWeek = DateUtil.getCurrentDateTime(historicalCandle.getPreSignal().getTimestamp()).getDayOfWeek();
				DayOfWeek currentSignalDayOfWeek = DateUtil.getCurrentDateTime(instantSignal.getTimestamp()).getDayOfWeek();
				if(preSignalDayOfWeek != DayOfWeek.FRIDAY && currentSignalDayOfWeek != DayOfWeek.MONDAY){
					// control the candles size of StringBuilder in order to prevent out of memory exception about java heap space
					int count = DateUtil.getDValueBetweenTwoTimestamp(historicalCandle.getEndTime(), currentTime, granularity);
					count = count > maxSize ? maxSize : count;
					for(int i = count -1; i > -1; i --){
				    	historicalCandle.setTime(getHistoricalCandleTime(currentTime, i, granularity));
				    	String batchInsertOperations = key + HistoricalCandleConstant.INSERT_INDEX + HistoricalCandleConstant.SPLITTER + JSON.toJSONString(historicalCandle);
					    redisOperations.add(batchInsertOperations);
				    }
				}
			}
			Candle candle = initHistoricalCandle(instantSignal, granularity, 0);
			String insertOperation = key + HistoricalCandleConstant.INSERT_INDEX + HistoricalCandleConstant.SPLITTER + JSON.toJSONString(candle);
			redisOperations.add(insertOperation);
		}
		return redisOperations;
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

	private Candle initHistoricalCandle(InstantSignal instantSignal, String granularity, int isFirstComeIn){
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
		candle.setGranularity(granularity);
		candle.setTime(DateUtil.getMarketEndTimeStrByGranularity(instantSignal.getTimestamp(), granularity));
		return CandleUtil.formatCandle(candle);
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
	
	public String getHistoricalCandleTime(long signalTime, int count, String granularity){
		return DateUtil.getPreMarketEndTime(signalTime, count, granularity);
	}
	
}
