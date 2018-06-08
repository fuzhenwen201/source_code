package com.broctagon.centrallogging.receive;

import org.springframework.amqp.core.Message;
import org.springframework.stereotype.Service;

@Service
public class ControlListener {

//	@RabbitListener()
	public void receiveMessage(Message message){
		// TODO handle the message to decide which to live, which to be dead
	}
}
