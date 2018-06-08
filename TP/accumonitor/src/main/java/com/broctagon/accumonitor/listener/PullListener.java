package com.broctagon.accumonitor.listener;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.service.PullService;
import com.broctagon.tradtnode.rabbit.message.MessageHelper;

@Service
public class PullListener {

	@Autowired
	private PullService pullService;
	
	@RabbitListener(queues="PULL.#")
	public void receiveMessage(Message message){
		pullService.addPull(MessageHelper.parseMessage(message.getBody()));
	}
}
