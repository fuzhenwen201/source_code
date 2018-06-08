package com.broctagon.trad.rm.obj.rs.rabbitmq;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component("verifierMessageRecv")
public class VerifierMessageRecv implements MessageListener{
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private VerifierMessageAnalyse testMessageAnalyse;
	
	public void onMessage(Message message) {
		try{			
			byte[] body = message.getBody();
			testMessageAnalyse.dispatchMessage(body);
		}
		catch(Exception e){	
			logger.error(e.getMessage());
		}
	}
	
}
