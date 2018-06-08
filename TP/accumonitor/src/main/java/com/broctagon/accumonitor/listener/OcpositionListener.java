package com.broctagon.accumonitor.listener;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.service.OCPositionService;
import com.broctagon.tradtnode.rabbit.message.MessageHelper;

@Service
public class OcpositionListener {

	@Autowired
	private OCPositionService ocPosition;
	
	@RabbitListener(queues = "OCPOSITION.#")
	public void receiveMessage(Message message){
		ocPosition.addOCPosition(MessageHelper.parseMessage(message.getBody()));
	}
}
