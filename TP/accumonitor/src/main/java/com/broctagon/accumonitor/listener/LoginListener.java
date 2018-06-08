package com.broctagon.accumonitor.listener;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.service.LoginService;
import com.broctagon.tradtnode.rabbit.message.MessageHelper;

@Service
public class LoginListener {

	@Autowired
	private LoginService loginService;

	@RabbitListener(queues = "LOGIN.#")
	public void receiveMessage(Message message){
		loginService.addLoginAns(MessageHelper.parseMessage(message.getBody()));
	}
	
}
