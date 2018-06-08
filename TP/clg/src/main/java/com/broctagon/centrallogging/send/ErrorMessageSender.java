package com.broctagon.centrallogging.send;
/**
 * send message to MQ for test
import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.centrallogging.prop.MQProperties;


@Component
public class ErrorMessageSender{
	
	@Autowired
	private MQProperties mqProperties;

	@Autowired
	private RabbitTemplate rabbitTemplate;
	
	public void send(Message message){
		rabbitTemplate.convertAndSend(mqProperties.getExchange(), mqProperties.getErrorRoutingKey(), message);
	}
	
}
 */
