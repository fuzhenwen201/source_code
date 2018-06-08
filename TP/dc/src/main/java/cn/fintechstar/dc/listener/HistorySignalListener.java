package cn.fintechstar.dc.listener;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import cn.fintechstar.dc.constant.Tag;
import cn.fintechstar.dc.model.HistoricalSignalRequest;
import cn.fintechstar.dc.service.HistoricalSignalService;
import cn.fintechstar.dc.tnode.TNode;
import cn.fintechstar.dc.tnode.TNodeCodec;
import cn.fintechstar.dc.util.HistoricalSignalMqCodec;

@Service
public class HistorySignalListener {
	
	@Value("${mq.queue.historySignalAns}")
	private String historicalSignalAns;
	
	@Value("${mq.signalExchange}")
	private String exchange;
	
	@Value("${maxCandlesOfSmallPacket}")
	private int maxCandlesofSmallPacket;
	
	@Value("${maxCandlesOfLargePacket}")
	private int maxCandlesOfLargePacket;
	
	@Autowired
	private RabbitTemplate rabbitTemplate;

	@Autowired
	private HistoricalSignalService historicalSignalService;
	
	private final Logger logger = Logger.getLogger(HistorySignalListener.class);
	
	@RabbitListener(queues = "HISTDATA_REQ_t")
	public void receiveMessage(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		HistoricalSignalRequest historicalSignalRequest = null;
		if(tnode.getTag() == Tag.HISTORY_CANDLES_REQ_SMALL_PACKET){
			historicalSignalRequest = HistoricalSignalMqCodec.historicalSignalRequestDecoder(tnode.getBody());
			logger.info("Received Getting Historical Request : " + historicalSignalRequest);
			int candlesCount = historicalSignalRequest.getCandlesCount() > maxCandlesofSmallPacket ? maxCandlesofSmallPacket: historicalSignalRequest.getCandlesCount();
			historicalSignalRequest.setCandlesCount(candlesCount);
		}else if(tnode.getTag() == Tag.HISTORY_CANDLES_REQ_LARGE_PACKET){
			historicalSignalRequest = HistoricalSignalMqCodec.historicalSignalRequestDecoder(tnode.getBody());
			logger.info("Received Getting Historical Request : " + historicalSignalRequest);
			int candlesCount = historicalSignalRequest.getCandlesCount() > maxCandlesOfLargePacket ? maxCandlesOfLargePacket : historicalSignalRequest.getCandlesCount();
			historicalSignalRequest.setCandlesCount(candlesCount);
		}
		if (historicalSignalRequest != null) {
			byte[] historicalSignal = null;
			if(historicalSignalRequest.getInstrument() != null && historicalSignalRequest.getGranularity() != null){
				historicalSignal = historicalSignalService.getHistoricalSignal(historicalSignalRequest, tnode);
			}else{
				historicalSignal = historicalSignalService.getTnodeMessage(historicalSignalRequest, null, tnode);
			}
			String responseQueue = historicalSignalAns + historicalSignalRequest.getMsgId();
			logger.info("Sending Historical Candles to exchange : " + exchange + ", routingKey : " + responseQueue);
			rabbitTemplate.convertAndSend(exchange, responseQueue,  new Message(historicalSignal, new MessageProperties()));
		}
	}
	
}
