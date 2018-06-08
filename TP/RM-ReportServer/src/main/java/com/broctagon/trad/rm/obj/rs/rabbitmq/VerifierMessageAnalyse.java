package com.broctagon.trad.rm.obj.rs.rabbitmq;



import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.rs.rabbitmq.RabbitMQHelper;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;


// the class is used for Test Message
@Component
public class VerifierMessageAnalyse {
	
	private static Logger logger = Logger.getLogger(VerifierMessageAnalyse.class);
	
	@Autowired private RabbitMQHelper rabbitMQHelper;
	
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
					
			// beneath is validation code
			case CommonConstants.TAG_UPDATE_TRADER_BALANCE:
			logger.info("CommonConstants.MESSAGE_TAG_UPDATE_TRADER_BALANCE_RM");
		//	rabbitMQHelper.check(message.getContent());
			break;
			
			case CommonConstants.TAG_GET_POSITIONS_ANS:
			logger.info("CommonConstants.MESSAGE_TAG_GET_POSITIONS_ANS");
		//	rabbitMQHelper.check(message.getContent());
			break;
			
			
			case CommonConstants.TAG_UPDATE_POSITION_ONROOT:
			logger.info("CommonConstants.TAG_UPDATE_POSITION_ONROOT");
		//	rabbitMQHelper.check(message.getContent());
			break;
			
			case CommonConstants.TAG_UPDATE_OPEN_TRADE_ONROOT:
			logger.info("CommonConstants.TAG_UPDATE_OPEN_TRADE_ONROOT");
		//	rabbitMQHelper.check(message.getContent());
			break;
			
			case CommonConstants.TAG_UPDATE_CLOSED_TRADE_ONROOT:
			logger.info("CommonConstants.TAG_UPDATE_CLOSED_TRADE_ONROOT");
		//	rabbitMQHelper.check(message.getContent());
			break;
			
			case CommonConstants.TAG_GET_POSITIONS_ANS_CMT:
			logger.info("CommonConstants.TAG_GET_POSITIONS_ANS_CMT");
			rabbitMQHelper.check(message.getContent());
			break;
						
			default:
				logger.info("no right tag"); 
			break;
		
		}
		
	}
	
}
