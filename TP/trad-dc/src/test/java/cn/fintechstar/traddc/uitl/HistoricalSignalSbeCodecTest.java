package cn.fintechstar.traddc.uitl;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.agrona.concurrent.UnsafeBuffer;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import cn.fintechstar.traddc.model.Candle;
import cn.fintechstar.traddc.model.Candles;
import cn.fintechstar.traddc.model.Granularity;
import cn.fintechstar.traddc.model.Instrument;
import cn.fintechstar.traddc.util.HistoricalSignalSbeCodec;

public class HistoricalSignalSbeCodecTest {
	
	private Candles candles;

	@Before
	public void init(){
		candles = new Candles();
		List<Candle> candleList = new ArrayList<Candle>();
		
		candles.setGranularity(Granularity.D);
		candles.setInstrument(Instrument.AUD_CAD);
		
		Candle candle = new Candle();
		candle.setTime(new Date().toString());
		candle.setOpenBid(1.092850);
		candle.setOpenAsk(1.093220);
		candle.setHighBid(1.121140);
		candle.setHighAsk(1.121250);
		candle.setLowBid(1.092210);
		candle.setLowAsk(1.092330);
		candle.setCloseBid(1.120280);
		candle.setCloseAsk(1.121080);
		candle.setVolume(427636);
		candle.setComplete(true);
		
		Candle candle2 = new Candle();
		candle2.setTime(new Date().toString());
		candle2.setOpenBid(1.119120);
		candle2.setOpenAsk(1.120120);
		candle2.setHighBid(1.126780);
		candle2.setHighAsk(1.126900);
		candle2.setLowBid(1.116090);
		candle2.setLowAsk(1.162100);
		candle2.setCloseBid(1.122690);
		candle2.setCloseAsk(1.122820);
		candle2.setVolume(277304);
		candle2.setComplete(false);
		
		
		Candle candle3 = new Candle();
		
		candleList.add(candle);
		candleList.add(candle2);
		candleList.add(candle3);
		candles.setCandleList(candleList);
	}
	
	@Test
	public void testCodec(){
		byte[] directbyte = HistoricalSignalSbeCodec.encode(candles, 10240000);
		UnsafeBuffer directBuffer = new UnsafeBuffer(directbyte);
		
		Candles candlesDecoded = HistoricalSignalSbeCodec.decodeData(directBuffer, 0);
		Assert.assertEquals(candles.getGranularity(), candlesDecoded.getGranularity());
		Assert.assertEquals(candles.getInstrument(), candlesDecoded.getInstrument());
		List<Candle> candleList = candles.getCandleList();
		List<Candle> candleListDecoded = candlesDecoded.getCandleList();
		Assert.assertEquals(candleList.size(), candleListDecoded.size());
		for(int i = 0 ; i < candleList.size(); i ++){
			Candle candle = candleList.get(i);
			Candle candleDecoded = candleListDecoded.get(i);
			if (candle.getTime() != null) {
				Assert.assertEquals(candle.getTime(), candleDecoded.getTime());
			}else{
				Assert.assertEquals("", candleDecoded.getTime());
			}
			Assert.assertEquals("number error", candle.getOpenBid(), candleDecoded.getOpenBid(), 0.0);
			Assert.assertEquals("number error", candle.getOpenAsk(), candleDecoded.getOpenAsk(), 0.0);
			Assert.assertEquals("number error", candle.getHighBid(), candleDecoded.getHighBid(), 0.0);
			Assert.assertEquals("number error", candle.getHighAsk(), candleDecoded.getHighAsk(), 0.0);
			Assert.assertEquals("number error", candle.getLowBid(), candleDecoded.getLowBid(), 0.0);
			Assert.assertEquals("number error", candle.getLowAsk(), candleDecoded.getLowAsk(), 0.0);
			Assert.assertEquals("number error", candle.getCloseBid(), candleDecoded.getCloseBid(), 0.0);
			Assert.assertEquals("number error", candle.getCloseAsk(), candleDecoded.getCloseAsk(), 0.0);
			Assert.assertEquals(candle.getVolume(), candleDecoded.getVolume());
			Assert.assertEquals(candle.isComplete(), candleDecoded.isComplete());
		}
	}

}
