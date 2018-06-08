package cn.fintechstar.dc.service;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;

import cn.fintechstar.dc.constant.Tag;
import cn.fintechstar.dc.dao.HistoricalSignalDao;
import cn.fintechstar.dc.model.Candle;
import cn.fintechstar.dc.model.Candles;
import cn.fintechstar.dc.model.HistoricalSignalRequest;
import cn.fintechstar.dc.tnode.TNode;
import cn.fintechstar.dc.tnode.TNodeCodec;
import cn.fintechstar.dc.util.CommonUtil;
import cn.fintechstar.dc.util.HistoricalSignalMqCodec;
import cn.fintechstar.dc.util.HistoricalSignalSbeCodec;
import cn.fintechstar.dc.util.SpreadUtil;

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
	
	private Candles getHistoricalSignalWithGroupSpread(HistoricalSignalRequest historySignalRequest){
		LocalDateTime requestTime = LocalDateTime.now();
		Candles candles = getHistoricalCandles(historySignalRequest, requestTime);
		if (candles.getCandleList().size() <= 0) {
			throw new IllegalArgumentException("can not get history signal from from redis");
		}
		if(candles.getInstrument().toLowerCase().endsWith("bo")){
			candles = addGroupSpreadToHistoricalSignal(historySignalRequest, candles);
		}
		return candles;
	}

	private Candles getHistoricalCandles(HistoricalSignalRequest historicalSignalRequest, LocalDateTime requestTime){
		Candles candles = new Candles();
		String instrument = historicalSignalRequest.getInstrument();
		String instrumentFinal = instrument.endsWith("bo") ? instrument.substring(0, instrument.length() -2) : instrument;
        String granularity = historicalSignalRequest.getGranularity();
		candles.setInstrument(instrument);
		candles.setGranularity(granularity);
        int count = historicalSignalRequest.getCandlesCount();
        candles.setCandleList(getHistoricalCandles(count, requestTime, instrumentFinal, granularity));
        return candles;
	}

	private List<Candle> getHistoricalCandles(int count, LocalDateTime requestTime, String instrument, String granularity){
		List<Candle> candleList = getHistoricalCandlesOnWeekDays(count, requestTime, instrument, granularity);
		return candleList;
	}

	private List<Candle> getHistoricalCandlesOnWeekDays(int count, LocalDateTime requestTime, String instrument, String granularity){
		List<Candle> candles = new ArrayList<>();
		String tableName = CommonUtil.getHistoryKey(instrument, granularity);
		List<String> candleStr = redisTemplate.opsForList().range(tableName, 0, count - 1);
        for(String str : candleStr){
        	candles.add(JSON.parseObject(str, Candle.class));
        }
		return candles;
	}

	private Candles addGroupSpreadToHistoricalSignal(HistoricalSignalRequest historySignalRequest, Candles candles) {
		List<Candle> candleList = candles.getCandleList();
		int spread = historySignalDao.getGroupSpread(historySignalRequest.getGroupId(), historySignalRequest.getInstrument());
		if(spread > 0){
			candleList.stream().parallel().forEach((candle) ->{
				candle.setOpenAsk(SpreadUtil.calculateAskSpread(candle.getOpenAsk(), spread));
				candle.setOpenBid(SpreadUtil.calculateBidSpread(candle.getOpenBid(), spread));
				candle.setCloseAsk(SpreadUtil.calculateAskSpread(candle.getCloseAsk(), spread));
				candle.setCloseBid(SpreadUtil.calculateBidSpread(candle.getCloseBid(), spread));
				candle.setHighAsk(SpreadUtil.calculateAskSpread(candle.getHighAsk(), spread));
				candle.setHighBid(SpreadUtil.calculateBidSpread(candle.getHighBid(), spread));
				candle.setLowAsk(SpreadUtil.calculateAskSpread(candle.getLowAsk(), spread));
				candle.setLowBid(SpreadUtil.calculateBidSpread(candle.getLowBid(), spread));	
			});
		}
		return candles;
	}

	private byte[] prepareSbeHistoricalSignal(Candles candles) {
		return HistoricalSignalSbeCodec.encode(candles, BUFFER_SIZE);
	}
	
	public byte[] getTnodeMessage(HistoricalSignalRequest historicalSignalRequest, byte[] body, TNode tnode){
		byte[] historicalSignal = HistoricalSignalMqCodec.historicalSignalEncoder(historicalSignalRequest, body);
		tnode.setTag(Tag.HISTORY_SIGNAL_ANS);
		tnode.setBody(historicalSignal);
		tnode.setBodyLength(historicalSignal.length);
		return TNodeCodec.TNodeEncode(tnode);
	}
	
}
