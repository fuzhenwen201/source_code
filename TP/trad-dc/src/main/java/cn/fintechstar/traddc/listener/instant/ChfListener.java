package cn.fintechstar.traddc.listener.instant;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.model.InstantSignal;
import cn.fintechstar.traddc.service.InstantSignalService;
import cn.fintechstar.traddc.service.symbol.ChfService;
import cn.fintechstar.traddc.tnode.TNode;
import cn.fintechstar.traddc.tnode.TNodeCodec;
import cn.fintechstar.traddc.util.InstantSignalMqCodec;

@Service
public class ChfListener {

	@Autowired
	private InstantSignalService instantSignalService;
	
	@Autowired
	private ChfService chfService;
	
	@RabbitListener(queues="CHF_HKD.DC")
	public void chfHkdfListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		chfService.saveChfHkd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="CHF_JPY.DC")
	public void chfJpyfListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		chfService.saveChfJpy(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="CHF_ZAR.DC")
	public void chfZarfListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		chfService.saveChfZar(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}

}
