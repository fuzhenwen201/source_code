//package cn.fintechstar.traddc.listener;
//
//import org.springframework.amqp.core.Message;
//
//import Candles;
//import Granularity;
//import HistoricalSignalRequest;
//import Instrument;
//import TNode;
//import TNodeCodec;
//import BytesUtil;
//import cn.fintechstar.traddc.util.HistoricalSignalSbeCodec;
//
///**
// * 	
// *  Add the code to the RabbitConfig.java
// *	@Bean
// *	public Queue chTestQueue(){
// *		return new Queue("chTest", false, false, true);
// *	}
// *	
// *	@Bean
// *	public Binding chTestBinding(){
// *		return BindingBuilder.bind(chTestQueue()).to(signalExchange()).with("DATACENTRE_ANS.msg.1");
// *	}
// *
// */
////@Service
//public class HistoricalCandlesListenerTest {
//
////	@RabbitListener(queues = "chTest")
//	public void test(Message message){
//		TNode tNode = TNodeCodec.decodeMessage(message.getBody());
//		System.out.println(tNode);
//		HistoricalSignalRequest historicalSignalRequest = historicalSignalAnsDecoder(tNode.getBody());
//		Candles candles = HistoricalSignalSbeCodec.decodeData(historicalSignalRequest.getSbe());
//		System.out.println(candles);
//	}
//	
//	public static final int REQ_ID_LEN = 36, GROUP_ID_LEN = 4, CLIENT_ID_LEN = 4, CANDLES_NUM_LEN = 4, INSTRU_LEN = 8, 
//			GRAN_LEN = 4, BODY_LEN = 4, MSG_ID_LEN = 4;
//
//	
//	
//	public static HistoricalSignalRequest historicalSignalAnsDecoder(byte[] requestBody){
//
//		HistoricalSignalRequest historicalSignalRequest = new HistoricalSignalRequest();
//		byte[] reqIdBytes = new byte[REQ_ID_LEN];
//		byte[] groupIdBytes = new byte[GROUP_ID_LEN];
//		byte[] clientIdBytes = new byte[CLIENT_ID_LEN];
//		byte[] candlesCountBytes = new byte[CANDLES_NUM_LEN];
//		byte[] instrumentBytes = new byte[INSTRU_LEN];
//		byte[] granulartityBytes = new byte[GRAN_LEN];
//		byte[] bodylenBytes = new byte[BODY_LEN];
//		byte[] body = new byte[87];
//		
//		int messageSize = REQ_ID_LEN;
//		for(int i = 0; i < REQ_ID_LEN; i++){
//			reqIdBytes[i] = requestBody[i]; 
//		}
//		
//		for(int i = 0 ; i < GROUP_ID_LEN; i++){
//			groupIdBytes[i] = requestBody[messageSize + i];
//		}
//		
//		messageSize = messageSize + GROUP_ID_LEN;
//		for(int i = 0; i < CLIENT_ID_LEN; i++){
//			clientIdBytes[i] = requestBody[messageSize + i];
//		}
//		
//		messageSize = messageSize + CLIENT_ID_LEN;
//		for(int i = 0; i < CANDLES_NUM_LEN; i++){
//			candlesCountBytes[i] = requestBody[messageSize + i];
//		}
//		
//		messageSize = messageSize + CANDLES_NUM_LEN;
//		for(int i = 0; i < INSTRU_LEN; i++){
//			instrumentBytes[i] = requestBody[messageSize + i];
//		}
//		
//		messageSize = messageSize + INSTRU_LEN;
//		for(int i = 0; i < GRAN_LEN; i++){
//			granulartityBytes[i] = requestBody[messageSize + i];
//		}
//		
//		messageSize = messageSize + GRAN_LEN;
//		for(int i = 0; i < BODY_LEN; i++){
//			bodylenBytes[i] = requestBody[messageSize + i];
//		}
//		
//		messageSize = messageSize + BODY_LEN;
//		System.out.println(requestBody.length - messageSize);
//		for(int i = 0; i < requestBody.length - messageSize; i++){
//			body[i] = requestBody[messageSize + i];
//		}
//		
//		Instrument instrument = Instrument.valueOf(new String(instrumentBytes).trim());
//		Granularity granularity = Granularity.valueOf(new String(granulartityBytes).trim());
//
//		historicalSignalRequest.setReqId(new String(reqIdBytes).trim());
//		historicalSignalRequest.setGroupId(BytesUtil.bytesToIntBig(groupIdBytes));
//		historicalSignalRequest.setClientId(BytesUtil.bytesToIntBig(clientIdBytes));
//		historicalSignalRequest.setCandlesCount(BytesUtil.bytesToIntBig(candlesCountBytes));
//		historicalSignalRequest.setInstrument(instrument);
//		historicalSignalRequest.setGranularity(granularity);
//		historicalSignalRequest.setLen(BytesUtil.bytesToIntBig(bodylenBytes));
//		historicalSignalRequest.setSbe(body);
//		return historicalSignalRequest;
//	}
//		
//}
