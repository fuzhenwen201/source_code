package com.broctagon.accumonitor.listener;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.service.LogoutService;
import com.broctagon.tradtnode.rabbit.message.MessageHelper;


@Service
public class LogoutListener {
	
	@Autowired
	private LogoutService logoutService;

	@RabbitListener(queues = "LOGOUT.#")
	public void receiveMessage(Message message){
		logoutService.addLogout(MessageHelper.parseMessage(message.getBody()));
	}
}
