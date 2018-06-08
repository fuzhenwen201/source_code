package com.broctagon.trad.rm.obj.rs.rabbitmq;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageListener;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component("getPositionsReqRecv")
public class GetPositionsReqRecv implements MessageListener{
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private MessageAnalyse analyseMessage;
	
	public void onMessage(Message message) {
		try {
			byte[] body = message.getBody();
			analyseMessage.dispatchMessage(body);
		} catch (Exception e) {
			logger.error(e.getMessage());
		}
	}
	
}
