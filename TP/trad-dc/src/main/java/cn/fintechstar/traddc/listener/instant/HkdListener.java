package cn.fintechstar.traddc.listener.instant;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.model.InstantSignal;
import cn.fintechstar.traddc.service.InstantSignalService;
import cn.fintechstar.traddc.service.symbol.HkdService;
import cn.fintechstar.traddc.tnode.TNode;
import cn.fintechstar.traddc.tnode.TNodeCodec;
import cn.fintechstar.traddc.util.InstantSignalMqCodec;

@Service
public class HkdListener {
 
	@Autowired
	private InstantSignalService instantSignalService;
	
	@Autowired
	private HkdService hkdService;
	
	@RabbitListener(queues="HKD_JPY.DC")
	public void hkdJpyListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		hkdService.saveHkdJpy(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	
}
