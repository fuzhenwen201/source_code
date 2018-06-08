package com.broctagon.centrallogging.receive;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Service;

import com.broctagon.centrallogging.prop.MQProperties;
import com.broctagon.centrallogging.util.CircleArray;

@Service
public class ErrorMessageListener {

	Logger logger = Logger.getLogger(ErrorMessageListener.class);
	
	public static CircleArray<Message> circleArray1 = new CircleArray<Message>(MQProperties.circleQueueSize);
	
	public static CircleArray<Message> circleArray2 = new CircleArray<Message>(MQProperties.circleQueueSize);
	
	@RabbitListener(queues= "CLG.POSITION")
	public void receiveMessage(Message message){
		circleArray1.set(message);
	}
	
	@RabbitListener(queues= "CLG.POSITION")
	public void receiveMessage2(Message message){
		circleArray2.set(message);
	}
}
