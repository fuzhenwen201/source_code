package cn.fintechstar.admin.listener;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.google.gson.Gson;

import cn.fintechstar.admin.constant.ResponseTitle;
import cn.fintechstar.admin.constant.Tag;
import cn.fintechstar.admin.model.synch.message.SynchDataAns;
import cn.fintechstar.admin.service.SynchDataService;
import cn.fintechstar.admin.tnode.TNode;
import cn.fintechstar.admin.tnode.TNodeCodec;

@Service
public class SynchDataListener {

	private static final Logger LOGGER = Logger.getLogger(SynchDataListener.class);
	
	private static final Gson GSON = new Gson();
	
	@Autowired
	private SynchDataService synchDataService;
	
	@Autowired
	private RabbitTemplate rabbitTemplate;
	
	@RabbitListener(queues="SYNCHDATA.AD")
	public void receiveMessage(Message message){
		TNode tNode = TNodeCodec.decodeMessage(message.getBody());
		switch (tNode.getTag()) {
		case Tag.SYNCH_DATA_REQ:
			SynchDataAns synchDataAns = synchDataService.getSynchDataAns(tNode.getBody());
			TNode synchDataAnsTnode = TNodeCodec.getTNode(Tag.SYNCH_DATA_ANS, GSON.toJson(synchDataAns));
			rabbitTemplate.send(ResponseTitle.NC_TITLE, new Message(TNodeCodec.encodeMessage(null), new MessageProperties()));
			rabbitTemplate.send(ResponseTitle.SYNCH_TITLE + synchDataAns.getSgId(), new Message(TNodeCodec.encodeMessage(synchDataAnsTnode), new MessageProperties()));
			break;
		default:
			LOGGER.error("unknown tag from SYNCHDATA.AD.  tag:" + tNode.getTag() + "   --  message: " + tNode.getBody());
			break;
		}
	}
}
