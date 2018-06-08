package cn.fintechstar.traddc.listener.instant;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.model.InstantSignal;
import cn.fintechstar.traddc.service.InstantSignalService;
import cn.fintechstar.traddc.service.symbol.SymbolService;
import cn.fintechstar.traddc.tnode.TNode;
import cn.fintechstar.traddc.tnode.TNodeCodec;
import cn.fintechstar.traddc.util.InstantSignalMqCodec;

@Service
public class SymbolButtListener {
	
	@Autowired
	private InstantSignalService instantSignalService;
	
	@Autowired
	private SymbolService symbolService;
	
	@RabbitListener(queues="SymbolBUTT.DC")
	public void fincoinUsdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		symbolService.saveSymbolButt(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);	
	}
}
