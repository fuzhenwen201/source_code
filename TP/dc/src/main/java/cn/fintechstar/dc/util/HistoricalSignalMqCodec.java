package cn.fintechstar.dc.util;

import cn.fintechstar.dc.constant.Tag;
import cn.fintechstar.dc.model.HistoricalSignalRequest;

public class HistoricalSignalMqCodec {
	
	public static final int REQ_ID_LEN = 36, GROUP_ID_LEN = 4, CLIENT_ID_LEN = 4, CANDLES_NUM_LEN = 4, INSTRU_LEN = 8, 
			GRAN_LEN = 4, BODY_LEN = 4, MSG_ID_LEN = 4;
	public static final int REQ_LEN = REQ_ID_LEN + GROUP_ID_LEN + CLIENT_ID_LEN + CANDLES_NUM_LEN + INSTRU_LEN + GRAN_LEN + BODY_LEN ;
	
	public static byte[] historicalSignalEncoder(HistoricalSignalRequest historySignalRequest, byte[] historySignal){
		
		String reqId = historySignalRequest.getReqId();
		int groupId = historySignalRequest.getGroupId();
		int clientId = historySignalRequest.getClientId();
		int candlesCount = historySignalRequest.getCandlesCount();
		String instrument = historySignalRequest.getInstrument();
		String granularity = historySignalRequest.getGranularity();
		
		
		byte[] reqIdBytes = reqId == null ? null : reqId.getBytes();
		byte[] groupIdBytes = BytesUtil.intToBytesBig(groupId);
		byte[] clientIdBytes = BytesUtil.intToBytesBig(clientId); 
		byte[] candlesCountBytes = BytesUtil.intToBytesBig(candlesCount);
		byte[] instrumentBytes = instrument == null ? null: instrument.toString().getBytes();
		byte[] granulartityBytes = granularity == null ? null : granularity.toString().getBytes();		
		byte[] historySignalBytes = BytesUtil.intToBytesBig(historySignal == null ? 0 : historySignal.length);
		byte[] message = new byte[historySignal == null ? 0 + REQ_LEN : historySignal.length + REQ_LEN];
		
		int messageSize = REQ_ID_LEN;
		if(reqIdBytes != null){
			for(int i = 0 ; i < reqIdBytes.length; i++){
				message[i] = reqIdBytes[i];
			}
		}
		
		for(int i = 0; i < groupIdBytes.length; i++ ){
			message[REQ_ID_LEN + i] = groupIdBytes[i];
		}
		
		messageSize = messageSize + GROUP_ID_LEN;
		for(int i = 0; i < clientIdBytes.length; i++){
			message[messageSize + i] = clientIdBytes[i];
		}
		
		messageSize = messageSize + CLIENT_ID_LEN;
		for(int i = 0; i < candlesCountBytes.length; i++){
			message[messageSize + i] = candlesCountBytes[i];
		}
		
		messageSize = messageSize + CANDLES_NUM_LEN;
		if(instrumentBytes != null){
			for(int i = 0; i < instrumentBytes.length; i++){
				message[messageSize + i] = instrumentBytes[i];
			}
		}
		
		messageSize = messageSize + INSTRU_LEN;
		if(granulartityBytes != null){
			for(int i = 0; i < granulartityBytes.length; i++){	
				message[messageSize + i] = granulartityBytes[i];
			}
		}
		
		messageSize = messageSize + GRAN_LEN;
		for(int i = 0; i < historySignalBytes.length; i++){
			message[messageSize + i]= historySignalBytes[i];
		}
		
		messageSize = messageSize + BODY_LEN;
		if(historySignalRequest.getGranularity() == null){
			byte[] bodyLenBytes = BytesUtil.intToBytesBig(Tag.ERROR_GRAN);
			if(historySignal != null){
				for(int i = 0; i < bodyLenBytes.length; i++){
					message[messageSize + i] = bodyLenBytes[i];
				}
			}
		}else if(historySignalRequest.getInstrument() == null){
			byte[] bodyLenBytes = BytesUtil.intToBytesBig(Tag.ERROR_INST);
			if(historySignal != null){
				for(int i = 0; i < bodyLenBytes.length; i++){
					message[messageSize + i] = bodyLenBytes[i];
				}
			}
		}else{
			if(historySignal != null){
				for(int i = 0; i < historySignal.length; i++){
					message[messageSize + i] = historySignal[i];
				}
			}
		}
		
		return message;
	}
	

	public static HistoricalSignalRequest historicalSignalRequestDecoder(byte[] requestBody){
		HistoricalSignalRequest historySignalRequest = new HistoricalSignalRequest();
		byte[] reqIdBytes = new byte[REQ_ID_LEN];
		byte[] groupIdBytes = new byte[GROUP_ID_LEN];
		byte[] clientIdBytes = new byte[CLIENT_ID_LEN];
		byte[] candlesCountBytes = new byte[CANDLES_NUM_LEN];
		byte[] instrumentBytes = new byte[INSTRU_LEN];
		byte[] granulartityBytes = new byte[GRAN_LEN];
		byte[] msgIdBytes = new byte[MSG_ID_LEN];
		
		int messageSize = REQ_ID_LEN;
		for(int i = 0; i < REQ_ID_LEN; i++){
			reqIdBytes[i] = requestBody[i]; 
		}
		
		for(int i = 0 ; i < GROUP_ID_LEN; i++){
			groupIdBytes[i] = requestBody[messageSize + i];
		}
		
		messageSize = messageSize + GROUP_ID_LEN;
		for(int i = 0; i < CLIENT_ID_LEN; i++){
			clientIdBytes[i] = requestBody[messageSize + i];
		}
		
		messageSize = messageSize + CLIENT_ID_LEN;
		for(int i = 0; i < CANDLES_NUM_LEN; i++){
			candlesCountBytes[i] = requestBody[messageSize + i];
		}
		
		messageSize = messageSize + CANDLES_NUM_LEN;
		for(int i = 0; i < INSTRU_LEN; i++){
			instrumentBytes[i] = requestBody[messageSize + i];
		}
		
		messageSize = messageSize + INSTRU_LEN;
		for(int i = 0; i < GRAN_LEN; i++){
			granulartityBytes[i] = requestBody[messageSize + i];
		}
		
		messageSize = messageSize + GRAN_LEN;
		for(int i = 0;i < MSG_ID_LEN; i++){
			msgIdBytes[i] = requestBody[messageSize + i];
		}

		historySignalRequest.setReqId(new String(reqIdBytes).trim());
		historySignalRequest.setGroupId(BytesUtil.bytesToIntBig(groupIdBytes));
		historySignalRequest.setClientId(BytesUtil.bytesToIntBig(clientIdBytes));
		historySignalRequest.setCandlesCount(BytesUtil.bytesToIntBig(candlesCountBytes));
		historySignalRequest.setInstrument(new String(instrumentBytes).trim());
		historySignalRequest.setGranularity(new String(granulartityBytes).trim());
		historySignalRequest.setMsgId(BytesUtil.bytesToIntBig(msgIdBytes));
					
		return historySignalRequest;
	}
		
}
