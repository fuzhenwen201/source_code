package com.broctagon.tradstnode.rabbit.publish;



import org.apache.log4j.Logger;
import org.springframework.amqp.core.AmqpTemplate;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageDeliveryMode;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;

/**
 * @author imdadullah
 *
 */
@Component
@Scope("singleton")
public class Publisher {
	
	private static Logger LOGGER = Logger.getLogger(Publisher.class);
	
	/**
	 * @param routingKey
	 * @param body
	 * 
	 * This method published message to rabbitMq without exchange parameter because
	 * exchange is already mapped in rabbitTemplate of rabbitmq.xml with rabbitSender 
	 */
	public static void basicPublishAndmqTemplate(String routingKey, byte[] body,AmqpTemplate amqpTemplate) {
		try {
			//AmqpTemplate amqpTemplate = (AmqpTemplate) Listener.getPublisher("rabbitSender");
			MessageProperties messageProperties = new MessageProperties();
			messageProperties.setContentLength(body.length);
			messageProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);
			Message message = new Message(body, messageProperties);
			LOGGER.info(" <<<<<<<<<<<<<<<<<<< GOING TO PUBLISH MESSAGE >>>>>>>>>>>>>>>>>");
			amqpTemplate.convertAndSend(routingKey, message);
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		} catch (Throwable e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		}

	}

	/**
	 * @param exchange
	 * @param routingKey
	 * @param body
	 * 
	 * This method published message to rabbitMq with given exchange parameter
	 */
	public static void basicPublishByGivenExchange(String exchange, String routingKey, byte[] body,AmqpTemplate amqpTemplate) {
		try {
			MessageProperties messageProperties = new MessageProperties();
			messageProperties.setContentLength(body.length);
			messageProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);
			Message message = new Message(body, messageProperties);
			LOGGER.info(" <<<<<<<<<<<<<<<<<<< GOING TO PUBLISH MESSAGE WITH GIVEN EXCHANGE >>>>>>>>>>>>>>>>>");
			amqpTemplate.convertAndSend(exchange, routingKey, message);
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		} catch (Throwable e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		}

	}

	/**
	 * @param exchange
	 * @param routingKey
	 * @param body
	 * @param amqpTemplate
	 * 
	 * This method published message to rabbitMq with given exchange parameter and AmqpTemplate
	 */
	public static void basicPublishByGivenExchangeAndmqTemplate(String exchange, String routingKey, byte[] body, AmqpTemplate amqpTemplate) {
		try {
			MessageProperties messageProperties = new MessageProperties();
			messageProperties.setContentLength(body.length);
			messageProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);
			Message message = new Message(body, messageProperties);
			LOGGER.info(" <<<<<<<<<<<<<<<<<<< GOING TO PUBLISH MESSAGE WITH GIVEN EXCHANGE >>>>>>>>>>>>>>>>>");
			amqpTemplate.convertAndSend(exchange, routingKey, message);
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		} catch (Throwable e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		}

	}
}
