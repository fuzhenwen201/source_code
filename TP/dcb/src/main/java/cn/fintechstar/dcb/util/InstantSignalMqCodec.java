package cn.fintechstar.dcb.util;

import org.springframework.util.StringUtils;

import cn.fintechstar.dcb.constant.InstrumentConstant;
import cn.fintechstar.dcb.model.InstantSignal;

public class InstantSignalMqCodec {
	
	public static final int INSTRU_LEN = 4, STATUS_LEN = 4, BID_LEN = 4, ASK_LEN = 4, TIME_LEN = 8;
	public static final int RECV_LEN = INSTRU_LEN + STATUS_LEN + BID_LEN + ASK_LEN + TIME_LEN; 
	
	public static InstantSignal instantSignalDecoder(byte[] message){
		InstantSignal instantSignal = new InstantSignal();
		
		byte[] instrumentBytes = new byte[INSTRU_LEN];
		byte[] statusBytes = new byte[STATUS_LEN];
		byte[] bidBytes = new byte[BID_LEN];
		byte[] askBytes = new byte[ASK_LEN];
		byte[] timestampBytes = new byte[TIME_LEN];
		
		int messageSize = INSTRU_LEN;
		for(int i = 0; i < INSTRU_LEN; i++){
			instrumentBytes[i] = message[i];
		}
		
		for(int i = 0; i < STATUS_LEN; i++){
			statusBytes[i] = message[messageSize + i];
		}
		
		messageSize = messageSize + STATUS_LEN;
		for (int i = 0; i < BID_LEN; i++) {
			bidBytes[i] = message[messageSize + i];
		}
		
		messageSize = messageSize + BID_LEN;
		for(int i = 0; i < ASK_LEN; i++){
			askBytes[i] = message[messageSize + i];
		}
		
		messageSize = messageSize + ASK_LEN;
		for(int i = 0; i < TIME_LEN; i++){
			timestampBytes[i] = message[messageSize + i];
		}
		
		String instrument = InstrumentConstant.getIntToString().get(BytesUtil.bytesToIntBig(instrumentBytes));
		if(!StringUtils.isEmpty(instrument)){
			instantSignal.setInstrument(instrument );
		}
		instantSignal.setStatus(BytesUtil.bytesToIntBig(statusBytes));
		instantSignal.setBid(BytesUtil.bytesToIntBig(bidBytes));
		instantSignal.setAsk(BytesUtil.bytesToIntBig(askBytes));
		instantSignal.setTimestamp(BytesUtil.bytesToLong(timestampBytes));
		return instantSignal;
	}

}
