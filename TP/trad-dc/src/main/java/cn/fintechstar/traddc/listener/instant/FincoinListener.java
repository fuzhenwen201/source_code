package cn.fintechstar.traddc.listener.instant;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import cn.fintechstar.traddc.dao.instant.FincoinUsdDao;
import cn.fintechstar.traddc.model.InstantSignal;
import cn.fintechstar.traddc.service.InstantSignalService;
import cn.fintechstar.traddc.tnode.TNode;
import cn.fintechstar.traddc.tnode.TNodeCodec;
import cn.fintechstar.traddc.util.InstantSignalMqCodec;

@Service
public class FincoinListener {

	@Autowired
	private InstantSignalService instantSignalService;
	
	@Autowired
	private FincoinUsdDao fincoinUsdDao;

	@RabbitListener(queues="FINCOIN_USD.DC")
	public void fincoinUsdListener(Message message){
		TNode tnode = TNodeCodec.TNodeDecode(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		fincoinUsdDao.saveFincoinUsd(instantSignal);
		instantSignalService.getHistoricalCandles(instantSignal);
	}
	
}
