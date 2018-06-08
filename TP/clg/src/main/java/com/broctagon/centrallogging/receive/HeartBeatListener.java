package com.broctagon.centrallogging.receive;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.centrallogging.constant.Tag;
import com.broctagon.centrallogging.model.HeartBeat;
import com.broctagon.centrallogging.prop.MQProperties;
import com.broctagon.centrallogging.service.TagErrorService;
import com.broctagon.tradtnode.rabbit.message.MessageHelper;
import com.fasterxml.jackson.databind.ObjectMapper;

@Service
public class HeartBeatListener {

	@Autowired
	private MQProperties mqProperties;
	
	@Autowired
	private TagErrorService tagErrorService;
	
	@Autowired
	private RabbitTemplate rabbitTemplate;
	
	@RabbitListener(queues= "MONITOR.CLG")
	public void receiveHeartBeat(Message message) throws Throwable{
		com.broctagon.tradtnode.rabbit.message.Message tnodeMessage = MessageHelper.parseMessage(message.getBody());
		if(Tag.MONITOR_TAG_SEND == tnodeMessage.Tag){
			HeartBeat heartBeat = new ObjectMapper().readValue(tnodeMessage.getMessageString(), HeartBeat.class);
			byte[] body = tagErrorService.getBody(heartBeat);
			rabbitTemplate.setQueue("MONITORANS." + heartBeat.getMid());
			rabbitTemplate.convertAndSend(mqProperties.getExchange(), "MONITORANS." + heartBeat.getMid(), new Message(body, new MessageProperties()));;
		}else{
			tagErrorService.saveTagError(tnodeMessage);
		}
	}

}
