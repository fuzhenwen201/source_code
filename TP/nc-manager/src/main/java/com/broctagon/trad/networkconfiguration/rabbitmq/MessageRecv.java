package com.broctagon.trad.networkconfiguration.rabbitmq;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;


@Component("messageRecv")
public class MessageRecv implements  MessageListener{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private MessageDispatcher messageDispatcher;
	
	@Override
	public void onMessage(Message message){		
		try{
			logger.info("message:" + message);
			messageDispatcher.dispatchMessage(message.getBody());
		}
		catch(Exception e){
			logger.error(e.getMessage());
			e.printStackTrace();
		}

	}
	
}
