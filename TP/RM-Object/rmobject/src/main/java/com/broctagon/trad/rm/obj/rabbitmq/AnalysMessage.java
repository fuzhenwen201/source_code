package com.broctagon.trad.rm.obj.rabbitmq;

import java.util.Date;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.service.BoReqService;
import com.broctagon.trad.rm.obj.service.TraderHistoryService;
import com.broctagon.trad.rm.obj.serviceimpl.EchoMessageService;
import com.broctagon.tradstnode.rabbit.message.Message;
import com.broctagon.tradstnode.rabbit.utils.DateUtils;

@Component
public class AnalysMessage {

	private static Logger LOGGER = Logger.getLogger(AnalysMessage.class);
	@Autowired
	private BoReqService boReqService;
	@Autowired
	private TraderHistoryService historyService;
	@Autowired
	private EchoMessageService echoservice;

	@SuppressWarnings("unused")
	public void checkMessage(Message message) {
		LOGGER.info("<<<GOING TO PROCESS MSG RECEIVED BY RMLISTENER WITH TAG >>>[" + message.getTag() +"]");
		if (message == null) {
			return;
		}
		
		String time = DateUtils.convertDateToString(new Date(), DateUtils.DdMmYyyyHmsFormatWithZone);
		switch (message.tag) {

		case CommonConstants.GET_TOP5BO_REQ_TAG:    		// Top five bo subscribe 771
			boReqService.addTopFive5BoReq(message.getMessageString().trim());
			break;
		
		  case CommonConstants.GET_TOP5BO_REMOVE_REQ_TAG:   // top five bo unsubscribe  777
		  boReqService.unSubTopFive5BoReq(message.getMessageString().trim()); 
		  break;
		 
		case CommonConstants.GET_CUSTOMBO_REQ_TAG:         //  customize bo subscribe 773
			boReqService.addCustomBoReq(message.getMessageString().trim());
			break;
		
		case CommonConstants.GET_CUSTOMBO_REMOVE_REQ_TAG:  //  customize bo unsubscribe  785
			boReqService.unSubCustomBoReq(message.getMessageString().trim()); 
			break;
		 
		case CommonConstants.GET_SYMBOLBO_REQ_TAG:        // Symbol Bo Subscribe 775
			boReqService.addSymbolBoReq(message);
			break;
		
		case CommonConstants.GET_SYMBOLBO_REMOVE_REQ_TAG: //Symbol Bo unsubscribe  787
			boReqService.unSubSymbolBoReq(message); 
			  break; 
		case CommonConstants.TRADER_HISTORY_REQ_TAG:      //Trader History  83
			historyService.tradersHistory(message, 83);
			break;
		case CommonConstants.PUB_ECHO_MSG_TAG:            // Echo message -24575
			echoservice.echoMessage(message, false);
			break;
		default:
			break;
		}
	}
}
