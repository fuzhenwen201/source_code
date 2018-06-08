package cn.fintechstar.traddc.service;

import java.time.DayOfWeek;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;

import cn.fintechstar.traddc.constant.Tag;
import cn.fintechstar.traddc.dao.HistoricalSignalDao;
import cn.fintechstar.traddc.model.Candle;
import cn.fintechstar.traddc.model.Candles;
import cn.fintechstar.traddc.model.Granularity;
import cn.fintechstar.traddc.model.HistoricalSignalRequest;
import cn.fintechstar.traddc.model.Instrument;
import cn.fintechstar.traddc.tnode.TNode;
import cn.fintechstar.traddc.tnode.TNodeCodec;
import cn.fintechstar.traddc.util.CommonUtil;
import cn.fintechstar.traddc.util.DateUtil;
import cn.fintechstar.traddc.util.HistoricalSignalMqCodec;
import cn.fintechstar.traddc.util.HistoricalSignalSbeCodec;
import cn.fintechstar.traddc.util.SpreadUtil;

@Service
public class HistoricalSignalService {

	private static final int BUFFER_SIZE = 10485760;
	
	@Autowired
	private HistoricalSignalDao historySignalDao;
	
	@Autowired
	private StringRedisTemplate redisTemplate;

	public byte[] getHistoricalSignal(HistoricalSignalRequest historicalSignalRequest, TNode tnode){
		Candles candles = getHistoricalSignalWithGroupSpread(historicalSignalRequest);
		byte[] body = prepareSbeHistoricalSignal(candles);
		byte[] historicalSignal = getTnodeMessage(historicalSignalRequest, body, tnode);
		return historicalSignal;
	}
	
	public Candles getHistoricalSignalWithGroupSpread(HistoricalSignalRequest historySignalRequest){
		LocalDateTime requestTime = LocalDateTime.now();
		Candles candles = getHistoricalCandles(historySignalRequest, requestTime);
		if (candles.getCandleList().size() <= 0) {
			throw new IllegalArgumentException("can not get history signal from from redis");
		}
		candles = addGroupSpreadToHistoricalSignal(historySignalRequest, candles, requestTime);
		return candles;
	}

	private Candles getHistoricalCandles(HistoricalSignalRequest historicalSignalRequest, LocalDateTime requestTime){
		Candles candles = new Candles();
        Instrument instrument = historicalSignalRequest.getInstrument();
        Granularity granularity = historicalSignalRequest.getGranularity();
		candles.setInstrument(instrument);
		candles.setGranularity(granularity);
        int count = historicalSignalRequest.getCandlesCount();
        candles.setCandleList(getHistoricalCandles(count, requestTime, instrument, granularity));
        return candles;
	}

	private List<Candle> getHistoricalCandles(int count, LocalDateTime requestTime, Instrument instrument, Granularity granularity){
		List<Candle> candleList = new ArrayList<>();
		if(DayOfWeek.SATURDAY.equals(requestTime.getDayOfWeek())){
			candleList.addAll(getHistoricalCandlesOnSat(count, requestTime, instrument, granularity));
		}else if(DayOfWeek.SUNDAY.equals(requestTime.getDayOfWeek())){
			candleList.addAll(getHistoricalCandlesOnSun(count, requestTime, instrument, granularity));
		}else{
			candleList.addAll(getHistoricalCandlesOnWeekDays(count, requestTime, instrument, granularity));
		}
		return candleList;
	}

	private List<Candle> getHistoricalCandlesOnWeekDays(int count, LocalDateTime requestTime, Instrument instrument, Granularity granularity){
		List<Candle> candles = new ArrayList<>();
		String tableName = CommonUtil.getHistoryKey(instrument, granularity);
		List<String> candleStr = redisTemplate.opsForList().range(tableName, 0, count - 1);
        for(String str : candleStr){
        	candles.add(JSON.parseObject(str, Candle.class));
        }
		return candles;
	}

	private List<Candle> getHistoricalCandlesOnSat(int count, LocalDateTime requestTime, Instrument instrument, Granularity granularity){
		List<Candle> candles = new ArrayList<>();
		String tableName = CommonUtil.getHistoryKey(instrument, granularity);
		String candleStr = redisTemplate.opsForList().index(tableName, 0);
		candles.add(JSON.parseObject(candleStr, Candle.class));
		if(count - 1 > 0){
			List<String> candlesStr = redisTemplate.opsForList().range(tableName, 0, count - 2);
            for(String str : candlesStr){
            	candles.add(JSON.parseObject(str, Candle.class));
            }
		}
		return candles;
	}
	
	private List<Candle> getHistoricalCandlesOnSun(int count, LocalDateTime requestTime, Instrument instrument, Granularity granularity){
		List<Candle> candles = new ArrayList<>();
		String tableName = CommonUtil.getHistoryKey(instrument, granularity);
		String candleStr = redisTemplate.opsForList().index(tableName, 0);
		candles.add(JSON.parseObject(candleStr, Candle.class));
		candles.add(JSON.parseObject(candleStr, Candle.class));
		if(count - 2 > 0){
			List<String> candlesStr = redisTemplate.opsForList().range(tableName, 0, count - 4);
            for(String str : candlesStr){
            	candles.add(JSON.parseObject(str, Candle.class));
            }
		}
		return candles;
	}

	private Candles addGroupSpreadToHistoricalSignal(HistoricalSignalRequest historySignalRequest, Candles candles, LocalDateTime requestTime) {
		List<Candle> candleList = candles.getCandleList();
		for(int i = 0; i < candleList.size(); i++){
			String historicalCandleTime = DateUtil.getHistoricalSpreadTime(historySignalRequest.getGranularity(), requestTime, i);
			int spread = historySignalDao.getGroupSpread(historySignalRequest.getGroupId(), historicalCandleTime, historySignalRequest.getInstrument());
			if(spread > 0){
				Candle candle = candleList.get(i);
				candle.setOpenAsk(SpreadUtil.calculateAskSpread(candle.getOpenAsk(), spread));
				candle.setOpenBid(SpreadUtil.calculateBidSpread(candle.getOpenBid(), spread));
				candle.setCloseAsk(SpreadUtil.calculateAskSpread(candle.getCloseAsk(), spread));
				candle.setCloseBid(SpreadUtil.calculateBidSpread(candle.getCloseBid(), spread));
				candle.setHighAsk(SpreadUtil.calculateAskSpread(candle.getHighAsk(), spread));
				candle.setHighBid(SpreadUtil.calculateBidSpread(candle.getHighBid(), spread));
				candle.setLowAsk(SpreadUtil.calculateAskSpread(candle.getLowAsk(), spread));
				candle.setLowBid(SpreadUtil.calculateBidSpread(candle.getLowBid(), spread));	
			}
		}
		return candles;
	}

	public byte[] prepareSbeHistoricalSignal(Candles candles) {
		return HistoricalSignalSbeCodec.encode(candles, BUFFER_SIZE);
	}
	
	private byte[] getTnodeMessage(HistoricalSignalRequest historicalSignalRequest, byte[] body, TNode tnode){
		byte[] historicalSignal = HistoricalSignalMqCodec.historicalSignalEncoder(historicalSignalRequest, body);
		tnode.setTag(Tag.HISTORY_SIGNAL_ANS);
		tnode.setBody(historicalSignal);
		tnode.setBodyLength(historicalSignal.length);
		return TNodeCodec.TNodeEncode(tnode);
	}
	
}
