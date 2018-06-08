package com.broctagon.trad.rm.obj.rs.rabbitmq;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageDeliveryMode;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Component;

import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.common.CommonUtil;
import com.broctagon.tradstnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;


@Component("rabbitMQHelper")
public class RabbitMQHelper {
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired 
	private RabbitTemplate rabbitSender ;
	@Autowired
	private Environment environment;
	
	public void check(byte[] content){
		String contentStr = new String(content);
		logger.info("new String(content):" + contentStr);
	}

	
	public void publishSGHistoryPosition(byte[] content,int sgid){		
		byte[] body = prepareEntileData(content,CommonConstants.TAG_GET_POSITIONS_REQ_CMT);	
		publishMqWithPropertiesKeyAndSgid(body, CommonConstants.MQ_ROUTINGKEY_PREFIX_RM_GETPOSITION_ANS, Integer.toString(sgid));
	}
	
	public void publishGetAllOpenPositionAns(byte[] content, int sgid){
		byte[] body = prepareEntileData(content,CommonConstants.TAG_GET_POSITIONS_ANS_CMT);		
		publishMqWithPropertiesKeyAndSgid(body, CommonConstants.MQ_ROUTINGKEY_PREFIX_RM_GETPOSITION_ANS, Integer.toString(sgid));
	}
	
	public void publishLastestOpenPositionAns(byte[] content){		
		byte[] body = prepareEntileData(content,CommonConstants.TAG_UPDATE_OPEN_TRADE_ONROOT);	
		publishMqWithPropertiesKey(body, CommonConstants.MQ_KEY_RM_UPDATE_OPENTRADE_ROOT);
	}
	
	public void publishClosePositionAns(byte[] content){
		byte[] body = prepareEntileData(content,CommonConstants.TAG_UPDATE_CLOSED_TRADE_ONROOT);
		publishMqWithPropertiesKey(body,CommonConstants.MQ_KEY_RM_UPDATE_CLOSETRADE_ROOT);		
	}
	
	public void publishGetPositionAns(byte[] content){
		byte[] body = prepareEntileData(content,CommonConstants.TAG_GET_POSITIONS_ANS);	
		publishMqWithPropertiesKey(body, CommonConstants.MQ_KEY_RM_GET_ORDER_ANS);
	}
	
	public void publishUpdatePositionsAns(byte[] content){
		byte[] body = prepareEntileData(content,CommonConstants.TAG_UPDATE_POSITION_ONROOT);
		publishMqWithPropertiesKey(body, CommonConstants.MQ_KEY_RM_UPDATE_ORDER_ANS);	
	}
	
	public void publishUpdateTrader(byte[] content){	
		byte[] body = prepareEntileData(content,CommonConstants.TAG_UPDATE_TRADER_BALANCE);
		publishMqWithPropertiesKey(body, CommonConstants.MQ_KEY_RM_UPDATE_TRADE_BALANCE);		
	}
		
	private void publishMqWithPropertiesKey(byte[] body, String propertiesKey){		
		publishMqWithPropertiesKeyAndSgid(body, propertiesKey, "");
	}
	
	private void publishMqWithPropertiesKeyAndSgid(byte[] body, String propertiesKey, String sgid){		
		String routingKey = environment.getProperty(propertiesKey);
		if(!CommonUtil.isEmptyString(routingKey)){
			routingKey =  routingKey + sgid;
			if(body!=null){
	    		MessageProperties messageProperties = new MessageProperties();    		
	    		messageProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);	
	    		Message rabbitMsg = new Message(body,messageProperties);
	    		rabbitSender.send(routingKey,rabbitMsg);
	    		logger.info("routingKey:" + routingKey);
	    	}
		}
		else{
			logger.error("routingKey is null, the Propertykey is " + propertiesKey);
		}	
	}
		
	private byte[] prepareEntileData(byte[] content, short tag){		
		
		com.broctagon.tradstnode.rabbit.message.Message ansMessage = new com.broctagon.tradstnode.rabbit.message.Message();
		ansMessage.setContent(content);
		ansMessage.setTag(tag);
		ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		byte[] body = null;
		try {
			body = MessageHelper.prepareMessage(ansMessage);
		} catch (Throwable e) {
			logger.error(e.getMessage());
		}  
			
		return body;
	}

}
