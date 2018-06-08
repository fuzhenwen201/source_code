package com.broctagon.trad.networkconfiguration.rabbitmq;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageDeliveryMode;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.trad.networkconfiguration.common.CommonConstants;
import com.broctagon.tradstnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;


@Component("rabbitMQHelper")
public class RabbitMQHelper {
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired 
	private RabbitTemplate rabbitSender ;
	
	public void check(byte[] content){
		String contentStr = new String(content);
		logger.info("new String(content):" + contentStr);
	}
		
	// all the below publish is used to test as AD role  
	public void publishChangeNoticeGroupFx(byte[] content){
		byte[] body = prepareEntileData(content,RabbitConstants.TAG_NC_MANAGER_CHNAGENOTICE_GROUPFX);	
		publishMqWithPropertiesKey(body, RabbitConstants.MQ_BINDINGKEY_NC_MANAGER_NOTICE_ANS);
	}
	
	public void publishChangeNoticeSecurityFx(byte[] content){
		byte[] body = prepareEntileData(content,RabbitConstants.TAG_NC_MANAGER_CHNAGENOTICE_SECURITYFX);	
		publishMqWithPropertiesKey(body, RabbitConstants.MQ_BINDINGKEY_NC_MANAGER_NOTICE_ANS);
	}
	
	public void publishChangeNoticeGroupBo(byte[] content){
		byte[] body = prepareEntileData(content,RabbitConstants.TAG_NC_MANAGER_CHNAGENOTICE_GROUPBO);	
		publishMqWithPropertiesKey(body, RabbitConstants.MQ_BINDINGKEY_NC_MANAGER_NOTICE_ANS);
	}
	
	public void publishChangeNoticeSecurityBo(byte[] content){
		byte[] body = prepareEntileData(content,RabbitConstants.TAG_NC_MANAGER_CHNAGENOTICE_SECURITYBO);	
		publishMqWithPropertiesKey(body, RabbitConstants.MQ_BINDINGKEY_NC_MANAGER_NOTICE_ANS);
	}
	
	private void publishMqWithPropertiesKey(byte[] body, String routingKey){		
		publishMqWithPropertiesKeyAndSgid(body, routingKey, "");
	}
	
	private void publishMqWithPropertiesKeyAndSgid(byte[] body, String routingKey, String sgid){		
		routingKey =  routingKey + sgid;
		MessageProperties messageProperties = new MessageProperties();    		
		messageProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);	
		Message rabbitMsg = new Message(body,messageProperties);
		rabbitSender.send(routingKey,rabbitMsg);
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
