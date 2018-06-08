package cn.fintechstar.traddc.listener;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.constant.Tag;
import cn.fintechstar.traddc.model.HistoricalSignalRequest;
import cn.fintechstar.traddc.service.HistoricalSignalService;
import cn.fintechstar.traddc.tnode.TNode;
import cn.fintechstar.traddc.tnode.TNodeCodec;
import cn.fintechstar.traddc.util.HistoricalSignalMqCodec;

@Service
public class HistorySignalListener {
	
	@Value("${mq.queue.historySignalAns}")
	private String historicalSignalAns;
	
	@Value("${mq.signalExchange}")
	private String exchange;
	
	@Autowired
	private RabbitTemplate rabbitTemplate;

	@Autowired
	private HistoricalSignalService historicalSignalService;
	
	private final Logger logger = Logger.getLogger(HistorySignalListener.class);
	
	@RabbitListener(queues = "HISTDATA_REQ")
	public void receiveMessage(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		if (tnode.getTag() == Tag.HISTORY_CANDLES_REQ_SMALL_PACKET || tnode.getTag() == Tag.HISTORY_CANDLES_REQ_LARGE_PACKET) {
			HistoricalSignalRequest historicalSignalRequest = HistoricalSignalMqCodec.historicalSignalRequestDecoder(tnode.getBody());
			logger.info("Received Getting Historical Request : " + historicalSignalRequest);
			byte[] historicalSignal = historicalSignalService.getHistoricalSignal(historicalSignalRequest, tnode);
			String responseQueue = historicalSignalAns + historicalSignalRequest.getMsgId();
			logger.info("Sending Historical Candles to exchange : " + exchange + ", routingKey : " + responseQueue);
			rabbitTemplate.convertAndSend(exchange, responseQueue,  new Message(historicalSignal, new MessageProperties()));
		}
	}
}
