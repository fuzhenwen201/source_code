package cn.fintechstar.dcb.listener;


import java.util.concurrent.ConcurrentLinkedQueue;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Service;

import cn.fintechstar.dcb.constant.InstrumentConstant;
import cn.fintechstar.dcb.constant.SignalConstant;
import cn.fintechstar.dcb.model.InstantSignal;
import cn.fintechstar.dcb.tnode.TNode;
import cn.fintechstar.dcb.tnode.TNodeCodec;
import cn.fintechstar.dcb.util.InstantSignalMqCodec;

@Service
public class InstantSymbolListener {
	
	@RabbitListener(queues="REALTIME.DC")
	public void receiveMessage(Message message){
		TNode tnode = TNodeCodec.decodeMessage(message.getBody());
		InstantSignal instantSignal = InstantSignalMqCodec.instantSignalDecoder(tnode.getBody());
		SignalConstant.ALL_SIGNALS.add(instantSignal);
		int mod = InstrumentConstant.getStringToInt().get(instantSignal.getInstrument()) % 6;
		ConcurrentLinkedQueue<InstantSignal> instantSignalQueue = InstrumentConstant.getINSTRUMENT_TO_QUEUE().get(mod);
		instantSignalQueue.add(instantSignal);
	}
	
}
