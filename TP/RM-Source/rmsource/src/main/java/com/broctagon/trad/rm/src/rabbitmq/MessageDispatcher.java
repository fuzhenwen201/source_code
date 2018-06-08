package com.broctagon.trad.rm.src.rabbitmq;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.trad.rm.src.common.CommonConstants;
import com.broctagon.trad.rm.src.service.OnlineUserSetService;
import com.broctagon.trad.rm.src.service.OrderAnsService;
import com.broctagon.trad.rm.src.service.UserInfoService;
import com.broctagon.trad.rm.src.serviceimpl.UpdateUserInfoServiceImp;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;


@Component
public class MessageDispatcher {
	
	private static Logger logger = Logger.getLogger(MessageDispatcher.class);
	
	@Autowired
	private UserInfoService userInfoService;
	@Autowired
	private OrderAnsService orderAnsService;
	@Autowired
	private UpdateUserInfoServiceImp updateUserInfo;
	@Autowired
	private OnlineUserSetService onlineUserSetService;
	
	public void dispatchMessage(byte[] body){
		
		com.broctagon.tradstnode.rabbit.message.Message message = null;
		try{
			message = MessageHelper.parseMessage(body);		
	 //		logger.info("MessageTag = " + message.getTag() + " MessageString = " + message.getMessageString() + " ResponceQueue = "+ message.getResponceQueue());
		}
		catch(StringIndexOutOfBoundsException e){
			logger.error(e.getMessage());
			return;
		}
				
		switch(message.tag){			
			case CommonConstants.TAG_TRADER_LOGIN_ANS:
				logger.info("CommonConstants.TAG_TRADER_LOGIN_ANS");
				onlineUserSetService.addOnlineUser(message.getContent());
				break;
			case CommonConstants.TAG_TRADER_LOGOUT_ANS:
				logger.info("CommonConstants.TAG_TRADER_LOGOUT_ANS");
				onlineUserSetService.removeOnlineUser(message.getContent());
				break;
			case CommonConstants.TAG_TRADER_FORCELOGOUT_ANS:
			//	logger.info("CommonConstants.TAG_TRADER_FORCELOGOUT_ANS");
			//	onlineUserSetService.removeOnlineUser(message.getContent());
				break;
				
			case CommonConstants.TAG_GET_ONLINE_TRADERS_ANS:
			//	logger.info("CommonConstants.TAG_GET_ONLINE_TRADERS_ANS");
				onlineUserSetService.storeAllOnlineUsers(message.getContent());
				break;
				
			case CommonConstants.TAG_CHANGE_BALANCE:
				logger.info("CommonConstants.TAG_CHANGE_BALANCE");
			//	userInfoService.updateUserInfo(message.getContent());
				break;
		
			case CommonConstants.TAG_ONLINE_USERINFO:
				logger.info("CommonConstants.TAG_ONLINE_USERINFO");
				userInfoService.storeUserInfo(message.getContent());  
				break;
			
			case CommonConstants.TAG_OFFLINE_USERINFO:
				logger.info("CommonConstants.TAG_OFFLINE_USERINFO");
				userInfoService.storeUserInfo(message.getContent());   
				updateUserInfo.writeUserInfoToRedis(message);
				break;
				
			case CommonConstants.TAG_USERINFO:
				logger.info("CommonConstants.MESSAGE_TAG_USERINFO_RM");
				userInfoService.storeUserInfo(message.getContent());
				break;
			
			// order	
			case CommonConstants.TAG_ORDERANS:
				logger.info("CommonConstants.MESSAGE_TAG_ORDERANS_RM");
				orderAnsService.storeOrderAns(message.getContent());		
				break;	
			
			case CommonConstants.TAG_CHANGE_BALANCE_ORDER:
				logger.info("CommonConstants.TAG_CHANGE_BALANCE_ORDER");
				orderAnsService.storeOrderAns(message.getContent());
				break;	
			
			default:
				logger.info("error tag, tag="+message.tag);
				break;
		
		}
		
	}
	
	public void routMes(Message message){
		
		MessageProperties messageProperties = message.getMessageProperties();
		messageProperties.getMessageId();
	}
	
}
