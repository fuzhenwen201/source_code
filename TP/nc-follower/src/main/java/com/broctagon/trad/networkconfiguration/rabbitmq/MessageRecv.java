package com.broctagon.trad.networkconfiguration.rabbitmq;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageListener;
import org.springframework.stereotype.Component;


@Component("messageRecv")
public class MessageRecv implements  MessageListener{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Override
	public void onMessage(Message message){		
		logger.info("message:" + message);
		logger.info("new String(message.getBody()): " + new String(message.getBody()));
		
	}
	
}
