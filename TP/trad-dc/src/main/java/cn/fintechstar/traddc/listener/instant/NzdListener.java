package cn.fintechstar.traddc.listener.instant;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.model.InstantSignal;
import cn.fintechstar.traddc.service.InstantSignalService;
import cn.fintechstar.traddc.service.symbol.NzdService;
import cn.fintechstar.traddc.tnode.TNode;
import cn.fintechstar.traddc.tnode.TNodeCodec;
import cn.fintechstar.traddc.util.InstantSignalMqCodec;

@Service
public class NzdListener {

	@Autowired
	private InstantSignalService instantSignalService;
	
	@Autowired
	private NzdService nzdService;
	
	@RabbitListener(queues="NZD_CAD.DC")
	public void nzdCadListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		nzdService.saveNzdCad(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="NZD_CHF.DC")
	public void nzdChfListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		nzdService.saveNzdChf(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="NZD_HKD.DC")
	public void nzdHkdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		nzdService.saveNzdHkd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="NZD_JPY.DC")
	public void nzdJpyListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		nzdService.saveNzdJpy(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="NZD_SGD.DC")
	public void nzdSgdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		nzdService.saveNzdSgd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="NZD_USD.DC")
	public void nzdUsdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		nzdService.saveNzdUsd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}

}
