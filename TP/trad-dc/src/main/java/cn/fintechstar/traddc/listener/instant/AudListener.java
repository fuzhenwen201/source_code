package cn.fintechstar.traddc.listener.instant;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.model.InstantSignal;
import cn.fintechstar.traddc.service.InstantSignalService;
import cn.fintechstar.traddc.service.symbol.AudService;
import cn.fintechstar.traddc.tnode.TNode;
import cn.fintechstar.traddc.tnode.TNodeCodec;
import cn.fintechstar.traddc.util.InstantSignalMqCodec;

@Service
public class AudListener {

	@Autowired
	private InstantSignalService instantSignalService;
	
	@Autowired
	private AudService audService;
	
	@RabbitListener(queues="AUD_CAD.DC")
	public void audCadListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		audService.saveAudCad(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="AUD_CHF.DC")
	public void audChfListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		audService.saveAudChf(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="AUD_HKD.DC")
	public void audHkdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		audService.saveAudHkd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="AUD_JPY.DC")
	public void audJpyListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		audService.saveAudJpy(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="AUD_NZD.DC")
	public void audNzdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		audService.saveAudNzd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="AUD_SGD.DC")
	public void audSgdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		audService.saveAudSgd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="AUD_USD.DC")
	public void audUsdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		audService.saveAudUsd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
}
