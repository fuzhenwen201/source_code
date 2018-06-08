package com.broctagon.trad.rm.src.rabbitmq;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageDeliveryMode;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.trad.rm.src.common.CommonConstants;
import com.broctagon.tradstnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;


@Component("rabbitMQHelper")
public class RabbitMQHelper {
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired 
	private RabbitTemplate userInfoSender ;
	/*@Autowired
	private Environment environment;
	*/
	public void check(byte[] content){
		String contentStr = new String(content);
		logger.info("new String(content):" + contentStr);
	}
	
	public void publishOnlineUsersRequest(byte[] content){
		byte[] body = prepareEntileData(content, CommonConstants.TAG_GET_ONLINE_TRADERS_REQ);
		publishMqWithPropertiesKey(body,CommonConstants.MQ_KEY_GET_ONLINETRADER_REQ);
	}
	
	@SuppressWarnings("unused")
	private void publishMqWithPropertiesKeyAndSgid(byte[] body, String preRoutingKey, String sgid){		
		String routingKey =  preRoutingKey + sgid;	
		publishMqWithPropertiesKey(body, routingKey);
	}
	
	private void publishMqWithPropertiesKey(byte[] body, String routingKey){		
		MessageProperties messageProperties = new MessageProperties();    		
		messageProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);	
		Message rabbitMsg = new Message(body,messageProperties);
		userInfoSender.send(routingKey,rabbitMsg);
		logger.info("routingKey:" + routingKey);
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
