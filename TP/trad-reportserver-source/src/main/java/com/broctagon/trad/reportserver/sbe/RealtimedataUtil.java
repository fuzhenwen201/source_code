package com.broctagon.trad.reportserver.sbe;

import java.math.BigDecimal;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import org.agrona.DirectBuffer;
import org.agrona.concurrent.UnsafeBuffer;

import com.broctagon.trad.reportserver.sbe.RealtimedatasEncoder.RealtimeDataListEncoder;
import com.broctagon.trad.reportserver.sbe.MessageHeaderDecoder;
import com.broctagon.trad.reportserver.sbe.MessageHeaderEncoder;
import com.broctagon.trad.reportserver.sbe.RealtimedatasDecoder;
import com.broctagon.trad.reportserver.sbe.RealtimedatasDecoder.RealtimeDataListDecoder;
import com.broctagon.trad.reportserver.sbe.RealtimedatasEncoder;

import com.broctagon.trad.reportserver.model.RealtimeData;
import com.broctagon.trad.reportserver.model.RealtimeDatas;

public class RealtimedataUtil {
	private static final MessageHeaderEncoder MESSAGE_HEADER_ENCODER = new MessageHeaderEncoder();
	private static final MessageHeaderDecoder MESSAGE_HEADER_DECODER = new MessageHeaderDecoder();
	private static final RealtimedatasEncoder REALTIMEDATA_ENCODER = new RealtimedatasEncoder();
	private static final RealtimedatasDecoder REALTIMEDATA_DECODER = new RealtimedatasDecoder();

	public static byte[] encode(RealtimeDatas realtimeDatas, int bufferSize) {
		final ByteBuffer byteBuffer = ByteBuffer.allocate(bufferSize);
		final UnsafeBuffer directBuffer = new UnsafeBuffer(byteBuffer);
		final int encodingLengthPlusHeaders = encode(directBuffer, realtimeDatas);
		byteBuffer.limit(encodingLengthPlusHeaders);
		System.err.println(encodingLengthPlusHeaders);
		
		byte[] directByte = directBuffer.byteArray();
		byte[] resultByte = new byte[encodingLengthPlusHeaders];
		System.arraycopy(directByte, 0, resultByte, 0, encodingLengthPlusHeaders);
		return resultByte;		
	}

	private static int encode(UnsafeBuffer directBuffer, RealtimeDatas realtimeDatas) {
		REALTIMEDATA_ENCODER.wrapAndApplyHeader(directBuffer, 0, MESSAGE_HEADER_ENCODER);
		List<RealtimeData> realTimeDataList = realtimeDatas.getRealtimeDatas();
		RealtimeDataListEncoder realTimeDateListEncoder = REALTIMEDATA_ENCODER.realtimeDataListCount(realTimeDataList.size());
		for(RealtimeData realtimeData : realTimeDataList){
			realTimeDateListEncoder.next()
				.bid(realtimeData.getBid())
				.ask(realtimeData.getAsk())
				.status(realtimeData.getStatus())
				.timestamp(realtimeData.getTimestamp().longValue())
				.instrument(realtimeData.getInstrument());
		}
		return MESSAGE_HEADER_ENCODER.ENCODED_LENGTH + REALTIMEDATA_ENCODER.encodedLength();
	}
	
	public static RealtimeDatas decode(byte[] content){
		UnsafeBuffer directBuffer = new UnsafeBuffer(content);
		return decode(directBuffer,0);
	}
	
	public static RealtimeDatas decode(DirectBuffer directBuffer, int bufferOffset){
		MESSAGE_HEADER_DECODER.wrap(directBuffer, bufferOffset);
		final int templateId = MESSAGE_HEADER_DECODER.templateId();
		if(templateId != REALTIMEDATA_DECODER.TEMPLATE_ID){
			throw new IllegalStateException("Template ids do not match");
		}
		final int actingBlockingLength = MESSAGE_HEADER_DECODER.blockLength();
		final int actingVersion = MESSAGE_HEADER_DECODER.version();
		bufferOffset += MESSAGE_HEADER_DECODER.encodedLength();
		return decode(directBuffer, bufferOffset, actingBlockingLength, actingVersion);
	}
	
	public static RealtimeDatas decode(DirectBuffer directBuffer, int bufferOffset, int actingBlockingLength, int actingVersion){
		RealtimeDatas realtimeDatas = new RealtimeDatas();
		List<RealtimeData> realtimeDataList = new ArrayList<>();
		REALTIMEDATA_DECODER.wrap(directBuffer, bufferOffset, actingBlockingLength, actingVersion);
		for(final RealtimeDataListDecoder  realtimeDateDecoder : REALTIMEDATA_DECODER.realtimeDataList()){
			RealtimeData realtimeData = new RealtimeData();
			realtimeData.setAsk(realtimeDateDecoder.ask());
			realtimeData.setBid(realtimeDateDecoder.bid());
			realtimeData.setStatus((int)realtimeDateDecoder.status());
			realtimeData.setInstrument(realtimeDateDecoder.instrument());
			realtimeData.setTimestamp(new BigDecimal(realtimeDateDecoder.timestamp()));
			realtimeDataList.add(realtimeData);
		}
		realtimeDatas.setRealtimeDatas(realtimeDataList);
		return realtimeDatas;
	}
}
