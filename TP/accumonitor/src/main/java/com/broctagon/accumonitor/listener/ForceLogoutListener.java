package com.broctagon.accumonitor.listener;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.service.ForceLogoutService;
import com.broctagon.tradtnode.rabbit.message.MessageHelper;

@Service
public class ForceLogoutListener {

	@Autowired
	private ForceLogoutService forceLogoutService;
	
	@RabbitListener(queues = "FORCELOGOUT.#")
	public void receiveMessage(Message message){
		forceLogoutService.addForceLogout(MessageHelper.parseMessage(message.getBody()));
	}
}
