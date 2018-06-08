package com.broctagon.trad.networkconfiguration.rabbitmq;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.trad.networkconfiguration.service.AdminConfigService;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;


@Component
public class MessageDispatcher {
	
	private static Logger logger = Logger.getLogger(MessageDispatcher.class);

	@Autowired
	private AdminConfigService adminConfigService;
	
	public void dispatchMessage(byte[] body){
		
		com.broctagon.tradstnode.rabbit.message.Message message = null;
		try{
		//	logger.info("new String(body):"+new String(body));
			message = MessageHelper.parseMessage(body);		
	 	//	logger.info("MessageTag = " + message.getTag() + " MessageString = " + message.getMessageString() + " ResponceQueue = "+ message.getResponceQueue());
		}
		catch(StringIndexOutOfBoundsException e){
			logger.error(e.getMessage());
			return;
		}
				
		switch(message.tag){	
						
			case RabbitConstants.TAG_NC_MANAGER_CHNAGENOTICE:   
				logger.info("RabbitConstants.TAG_NC_MANAGER_CHNAGENOTICE");				
				adminConfigService.updateAll();
				break;
												
			default:logger.info("no right tag"); 
			break;
		
		}
		
	}
		
}
