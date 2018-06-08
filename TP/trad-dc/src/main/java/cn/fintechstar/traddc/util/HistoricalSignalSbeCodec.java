	package cn.fintechstar.traddc.util;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import org.agrona.DirectBuffer;
import org.agrona.concurrent.UnsafeBuffer;

import cn.fintechstar.traddc.constant.SbeMap;
import cn.fintechstar.traddc.model.Candle;
import cn.fintechstar.traddc.model.Candles;
import cn.fintechstar.traddc.sbe.CandlesDataDecoder;
import cn.fintechstar.traddc.sbe.CandlesDataDecoder.CandlesDecoder;
import cn.fintechstar.traddc.sbe.CandlesDataEncoder;
import cn.fintechstar.traddc.sbe.CandlesDataEncoder.CandlesEncoder;
import cn.fintechstar.traddc.sbe.MessageHeaderDecoder;
import cn.fintechstar.traddc.sbe.MessageHeaderEncoder;

public class HistoricalSignalSbeCodec {
	
	private static final MessageHeaderEncoder MESSAGE_HEADER_ENCODER = new MessageHeaderEncoder();
	private static final MessageHeaderDecoder MESSAGE_HEADER_DECODER = new MessageHeaderDecoder();
	private static final CandlesDataEncoder CANDLES_ENCODER = new CandlesDataEncoder();
	private static final CandlesDataDecoder CANDLES_DECODER = new CandlesDataDecoder();
	
	private static final int NUM_FORMAT = 1000000;

	public static byte[] encode(Candles candles, int bufferSize){
		final ByteBuffer byteBuffer = ByteBuffer.allocate(bufferSize);
		final UnsafeBuffer directBuffer = new UnsafeBuffer(byteBuffer);
		final int encodingLengthPlusHeader = encodeData(directBuffer, candles); 
		byteBuffer.limit(encodingLengthPlusHeader);
		byte[] directByte = directBuffer.byteArray();
		byte[] resultByte = new byte[encodingLengthPlusHeader];
		System.arraycopy(directByte, 0, resultByte, 0, encodingLengthPlusHeader);
		return resultByte;
	}

	public static int encodeData(final UnsafeBuffer directBuffer, final Candles candles) {
		CANDLES_ENCODER.wrapAndApplyHeader(directBuffer, 0, MESSAGE_HEADER_ENCODER);
		CANDLES_ENCODER.granularity(candles.getGranularity());
		CANDLES_ENCODER.instrument(candles.getInstrument());
		CandlesEncoder candlesEncoder= CANDLES_ENCODER.candlesCount(candles.getCandleList().size());
		for (Candle candle : candles.getCandleList()) {
			candlesEncoder.next()
				.openBid(getLongValue(candle.getOpenBid()))
				.openAsk(getLongValue(candle.getOpenAsk()))
				.highBid(getLongValue(candle.getHighBid()))
				.highAsk(getLongValue(candle.getHighAsk()))
				.lowBid(getLongValue(candle.getLowBid()))
				.lowAsk(getLongValue(candle.getLowAsk()))
				.closeBid(getLongValue(candle.getCloseBid()))
				.closeAsk(getLongValue(candle.getCloseAsk()))
				.volumn(candle.getVolume())
				.complete(SbeMap.getSbeBooleanType(candle.isComplete()));
			if (candle.getTime() != null) {
				candlesEncoder.time(candle.getTime());
			}
		}
		return MessageHeaderEncoder.ENCODED_LENGTH + CANDLES_ENCODER.encodedLength();
	}
	
	public static Candles decodeData(byte[] coreContent){
		UnsafeBuffer directBuffer = new UnsafeBuffer(coreContent);
		return HistoricalSignalSbeCodec.decodeData(directBuffer, 0);
	}
	
	public static Candles decodeData(DirectBuffer directBuffer, int bufferOffset){
		MESSAGE_HEADER_DECODER.wrap(directBuffer, bufferOffset);
        final int templateId = MESSAGE_HEADER_DECODER.templateId();
        if (templateId != CandlesDataEncoder.TEMPLATE_ID)
        {
            throw new IllegalStateException("Template ids do not match");
        }
        final int actingBlockingLength = MESSAGE_HEADER_DECODER.blockLength();
        final int actingVersion = MESSAGE_HEADER_DECODER.version();
        bufferOffset += MESSAGE_HEADER_DECODER.encodedLength();
        return decodeDate(directBuffer, bufferOffset, actingBlockingLength, actingVersion);
	}

	public static Candles decodeDate(DirectBuffer directBuffer, int bufferOffset, int actingBlockingLength, int actingVersion) {
		Candles candles = new Candles();
		CANDLES_DECODER.wrap(directBuffer, bufferOffset, actingBlockingLength, actingVersion);
		candles.setGranularity(CANDLES_DECODER.granularity());
		candles.setInstrument(CANDLES_DECODER.instrument());
		List<Candle> candleList = new ArrayList<>();
		for(final CandlesDecoder candlesDecoder : CANDLES_DECODER.candles()){
			Candle candle = new Candle();
			candle.setTime(candlesDecoder.time());
			candle.setOpenBid(getDoubleValue(candlesDecoder.openBid()));
			candle.setOpenAsk(getDoubleValue(candlesDecoder.openAsk()));
			candle.setHighBid(getDoubleValue(candlesDecoder.highBid()));
			candle.setHighAsk(getDoubleValue(candlesDecoder.highAsk()));
			candle.setLowBid(getDoubleValue(candlesDecoder.lowBid()));
			candle.setLowAsk(getDoubleValue(candlesDecoder.lowAsk()));
			candle.setCloseBid(getDoubleValue(candlesDecoder.closeBid()));
			candle.setCloseAsk(getDoubleValue(candlesDecoder.closeAsk()));
			candle.setVolume((int)candlesDecoder.volumn());
			candle.setComplete(SbeMap.getDcBooleanType(candlesDecoder.complete()));
			candleList.add(candle);
		}
		candles.setCandleList(candleList);
		return candles;
	}
	
	private static long getLongValue(double d){
		return new Double(d * NUM_FORMAT).longValue();
	}
	
	private static double getDoubleValue(long l){
		return new Long(l).doubleValue() / NUM_FORMAT;
	}

}
