package com.broctagon.trad.networkconfiguration.rabbitmq;

import java.util.UUID;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageDeliveryMode;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.tradstnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;

import cn.fintechstar.traddc.tnode.TNode;
import cn.fintechstar.traddc.tnode.TNodeCodec;


@Component("rabbitMQHelper")
public class RabbitMQHelper {
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired 
	private RabbitTemplate rabbitSender ;
	
	public void check(byte[] content){
		String contentStr = new String(content);
		logger.info("new String(content):" + contentStr);
	}
	
	// for old tnode version
	public void publishGroupSpreadChangeNotification_Old(byte[] content){
		byte[] body = prepareEntileData(content, RabbitConstants.TAG_NC_FOLLOWER_CHNAGENOTICE_GROUPSPREAD);
		publishMqWithPropertiesKey_Old(body,RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_GROUPSPREAD_OLD);
	}
	
	// for old tnode version
	public void publishBoSymbolChangeNotification_Old(byte[] content){
		byte[] body = prepareEntileData(content, RabbitConstants.TAG_NC_FOLLOWER_CHNAGENOTICE_BOSYMBOL);
		publishMqWithPropertiesKey_Old(body,RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_BOSYMBOL_OLD);
	}
	
		
	public void publishSymbolSpreadChangeNotification(byte[] content){
		byte[] body = prepareTnodeData(content, RabbitConstants.TAG_NC_FOLLOWER_CHNAGENOTICE_SYMBOLSPREAD);
		publishMqWithPropertiesKey(body,RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_SYMBOLSPREAD);
	}
	
	public void publishGroupSpreadChangeNotification(byte[] content){
		byte[] body = prepareTnodeData(content, RabbitConstants.TAG_NC_FOLLOWER_CHNAGENOTICE_GROUPSPREAD);
		publishMqWithPropertiesKey(body,RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_GROUPSPREAD);
	}
	
	public void publishUrlMapChangeNotification(byte[] content){
		byte[] body = prepareTnodeData(content, RabbitConstants.TAG_NC_FOLLOWER_CHNAGENOTICE_URLMAP);
		publishMqWithPropertiesKey(body,RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_URLMAP);
	}
	
	public void publishBoSymbolChangeNotification(byte[] content){
		byte[] body = prepareTnodeData(content, RabbitConstants.TAG_NC_FOLLOWER_CHNAGENOTICE_BOSYMBOL);
		publishMqWithPropertiesKey(body,RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_BOSYMBOL);
	}
	
	private void publishMqWithPropertiesKeyAndSgid(byte[] body, String preRoutingKey, String sgid){		
		String routingKey =  preRoutingKey + sgid;	
		publishMqWithPropertiesKey(body, routingKey);
	}
	
	private void publishMqWithPropertiesKey(byte[] body, String routingKey){		
		MessageProperties messageProperties = new MessageProperties();    		
		messageProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);	
		Message rabbitMsg = new Message(body,messageProperties);
		rabbitSender.send(RabbitConstants.MQ_EXCHANGE_NC,routingKey,rabbitMsg);
		logger.info("routingKey:" + routingKey);
	}
	
	private void publishMqWithPropertiesKey_Old(byte[] body, String routingKey){		
		MessageProperties messageProperties = new MessageProperties();    		
		messageProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);	
		Message rabbitMsg = new Message(body,messageProperties);
		rabbitSender.send(RabbitConstants.MQ_EXCHANGE_GRP,routingKey,rabbitMsg);
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
	
	private byte[] prepareTnodeData(byte[] content, short tag){
		TNode tNode = new TNode();
		
		UUID uuidRequestId = UUID.randomUUID();
		String requestId = uuidRequestId.toString().replaceAll("-", "").substring(0, 16);
		UUID uuidSessionId = UUID.randomUUID();
		String sessionId = uuidSessionId.toString().replaceAll("-", "").substring(0, 16);
		
		tNode.setRequestId(requestId);
		tNode.setSessionId(sessionId);
		tNode.setBody(content);
		tNode.setBodyLength(content.length);
		tNode.setTag(tag);
		byte[] body = TNodeCodec.TNodeEncode(tNode);
		return body;
	}
		

}
