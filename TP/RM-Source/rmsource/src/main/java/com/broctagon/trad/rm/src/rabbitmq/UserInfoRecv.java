package com.broctagon.trad.rm.src.rabbitmq;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component("userInfoRecv")
public class UserInfoRecv implements MessageListener{
	Logger logger = Logger.getLogger(UserInfoRecv.class);
	
	@Autowired
	private MessageDispatcher messageDispatcher;
	
	public void onMessage(Message message) {
		try{		
			byte[] body = message.getBody();
			messageDispatcher.dispatchMessage(body);
		}
		catch(Exception e){
			logger.error(e.getMessage());
		}
	}
	
}
