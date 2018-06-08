package com.broctagon.trad.rm.obj.rs.rabbitmq;



import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.rs.service.HistoryPositionService;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;


@Component
public class MessageAnalyse {
	
	private static Logger logger = Logger.getLogger(MessageAnalyse.class);
	
	@Autowired private HistoryPositionService historyPositionService;
//	@Autowired private UpdateTraderService updateTraderService;
	
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
			case CommonConstants.TAG_GET_POSITIONS_REQ:   
			logger.info("CommonConstants.TAG_GET_POSITIONS_REQ");
			historyPositionService.responseGetPositionsReq(message.getContent());
			break;
			
			case CommonConstants.TAG_GET_POSITIONS_REQ_CMT:   
			logger.info("CommonConstants.TAG_GET_POSITIONS_REQ_CMT");
			historyPositionService.responseGetAllOpenPosition(message.getContent());
			break;
			
			case CommonConstants.TAG_RM_CM_COMMUNICATION_ERQ:   
			logger.info("CommonConstants.TAG_RM_CM_COMMUNICATION_ERQ");
	//		rabbitMQHelper.check(message.getContent());	
			break;
			
			case CommonConstants.TAG_GET_TRADERS_REQ:   
			logger.info("CommonConstants.TAG_GET_TRADERS_REQ");
	//		updateTraderService.publishOnlineTraderBalance();
			break;
						
			default:logger.info("no right tag"); 
			break;
		
		}
		
	}
		
}
