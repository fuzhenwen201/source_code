package com.broctagon.accumonitor.listener;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.service.UserInfoService;
import com.broctagon.tradtnode.rabbit.message.MessageHelper;


@Service
public class UserInfoListener {
	
	@Autowired
	private UserInfoService userInfoService;

	@RabbitListener(queues="USERINFO.#")
	public void receiveMessage(Message message){
		userInfoService.addUserInfo(MessageHelper.parseMessage(message.getBody()));
	}
}
