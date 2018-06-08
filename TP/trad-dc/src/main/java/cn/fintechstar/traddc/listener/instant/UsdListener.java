package cn.fintechstar.traddc.listener.instant;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.model.InstantSignal;
import cn.fintechstar.traddc.service.InstantSignalService;
import cn.fintechstar.traddc.service.symbol.UsdService;
import cn.fintechstar.traddc.tnode.TNode;
import cn.fintechstar.traddc.tnode.TNodeCodec;
import cn.fintechstar.traddc.util.InstantSignalMqCodec;

@Service
public class UsdListener {
	
	@Autowired
	private InstantSignalService instantSignalService;
	
	@Autowired
	private UsdService usdService;
	
	@RabbitListener(queues="USD_CAD.DC")
	public void usdCadListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdCad(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_CHF.DC")
	public void usdChfListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdChf(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_CNH.DC")
	public void usdCnhListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdCnh(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_CZK.DC")
	public void usdCzkListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdCzk(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_DKK.DC")
	public void usdDkkListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdDkk(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_HKD.DC")
	public void usdHkdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdHkd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}	
	
	@RabbitListener(queues="USD_HUF.DC")
	public void usdHufListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdHuf(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_INR.DC")
	public void usdInrListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdInr(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_JPY.DC")
	public void usdJpyListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdJpy(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);	
	}
	
	@RabbitListener(queues="USD_MXN.DC")
	public void usdMxnListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdMxn(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_NOK.DC")
	public void usdNokListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdNok(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_PLN.DC")
	public void usdPlnListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdPln(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_SAR.DC")
	public void usdSarListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdSar(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_SEK.DC")
	public void usdSekListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdSek(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_SGD.DC")
	public void usdSgdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdSgdDao(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_THB.DC")
	public void usdThbListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdThb(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_TRY.DC")
	public void usdTryListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdTry(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
	@RabbitListener(queues="USD_ZAR.DC")
	public void usdZarListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		usdService.saveUsdZar(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
}
