package cn.fintechstar.traddc.listener.instant;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.model.InstantSignal;
import cn.fintechstar.traddc.service.InstantSignalService;
import cn.fintechstar.traddc.service.symbol.CadService;
import cn.fintechstar.traddc.tnode.TNode;
import cn.fintechstar.traddc.tnode.TNodeCodec;
import cn.fintechstar.traddc.util.InstantSignalMqCodec;

@Service
public class CadListener {
	
	@Autowired
	private InstantSignalService instantSignalService;

	@Autowired
	private CadService cadService;
	
	@RabbitListener(queues="CAD_CHF.DC")
	public void cadChfListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		cadService.saveCadChf(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="CAD_HKD.DC")
	public void cadHkdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		cadService.saveCadHkd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="CAD_JPY.DC")
	public void cadJpyListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		cadService.saveCadJpy(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="CAD_SGD.DC")
	public void cadSgdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		cadService.saveCadSgd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
}
