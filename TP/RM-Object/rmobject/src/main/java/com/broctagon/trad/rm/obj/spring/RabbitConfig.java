package com.broctagon.trad.rm.obj.spring;

import org.springframework.amqp.core.AcknowledgeMode;
import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.BindingBuilder;
import org.springframework.amqp.core.Queue;
import org.springframework.amqp.core.TopicExchange;
import org.springframework.amqp.rabbit.connection.CachingConnectionFactory;
import org.springframework.amqp.rabbit.connection.ConnectionFactory;
import org.springframework.amqp.rabbit.core.RabbitAdmin;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.amqp.rabbit.listener.SimpleMessageListenerContainer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.config.ConfigurableBeanFactory;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Scope;
import org.springframework.core.env.Environment;

import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.rabbitmq.RMListener;


@Configuration
public class RabbitConfig {

	@Autowired private Environment environment;
//	@Autowired private UserInfoRecv userInfoRecv;
	@Autowired private RMListener receivedMsg;
	
	@Bean
	public ConnectionFactory connectionFactory(){
		CachingConnectionFactory connectionFactory = new CachingConnectionFactory();
		connectionFactory.setAddresses(environment.getProperty("mq.host"));
		connectionFactory.setPort(Integer.parseInt(environment.getProperty("mq.port")));
		connectionFactory.setVirtualHost(environment.getProperty("mq.vhost"));
		connectionFactory.setUsername(environment.getProperty("mq.user"));
		connectionFactory.setPassword(environment.getProperty("mq.password"));
		connectionFactory.setPublisherConfirms(false);
		connectionFactory.setPublisherReturns(false);
		return connectionFactory;
	}

	@Bean
	public RabbitAdmin rabbitAdmin() {
		RabbitAdmin rabbitAdmin = new RabbitAdmin(connectionFactory());
		return rabbitAdmin;
	}
	
	@Bean
	@Scope(ConfigurableBeanFactory.SCOPE_PROTOTYPE)
	public RabbitTemplate rabbitSender() {
		RabbitTemplate rabbitTemplate = rabbitAdmin().getRabbitTemplate();
		rabbitTemplate.setExchange(environment.getProperty("mq.exchange"));
	//	rabbitTemplate.setRoutingKey(CommonConstants.MQ_KEY_RM_GETORDER_ANS);
		return rabbitTemplate;
	}
	
	@Bean
	public Queue getTop5BoReqQueue(){
		return new Queue(CommonConstants.GET_TOP5BO_REQ, false, false, true);
	}
	@Bean
	public Queue getCustomReqQueue(){
		return new Queue(CommonConstants.GET_CUSTOMBO_REQ, false, false, true);
	}
	@Bean
	public Queue getSymbolBoReqQueue(){
		return new Queue(CommonConstants.GET_SYMBOLBO_REQ, false, false, true);
	}
	@Bean
	public Queue getTop5BoRemoveReqQueue(){
		return new Queue(CommonConstants.GET_TOP5BO_REMOVE_REQ, false, false, true);
	}
	
	@Bean
	public Queue getCustomBoRemoveReqQueue(){
		return new Queue(CommonConstants.GET_CUSTOMBO_REMOVE_REQ, false, false, true);
	}
	
	@Bean
	public Queue getSymbolBoRemoveReqQueue(){
		return new Queue(CommonConstants.GET_SYMBOLBO_REMOVE_REQ, false, false, true);
	}
	
	@Bean
	public Queue cmTormComunicationQueue(){
		return new Queue(CommonConstants.RM_CM_COMMUNICATION_ERQ, false, false, true);
	}
	
	@Bean
	public Queue rmMonitorQueue(){
		return new Queue(CommonConstants.RM_MONITOR, false, false, true);
	}
	
	@Bean
	public TopicExchange topicExchange(){
		return new TopicExchange(environment.getProperty("mq.exchange"), false, false);
	}
	
	@Bean
	public Binding ogetTop5BoReqBinding(){
		return BindingBuilder.bind(getTop5BoReqQueue()).to(topicExchange()).with(CommonConstants.GET_TOP5BO_REQ);
	}	
	@Bean
	public Binding getCustomReqBinding(){
		return BindingBuilder.bind(getCustomReqQueue()).to(topicExchange()).with(CommonConstants.GET_CUSTOMBO_REQ);
	}
	@Bean
	public Binding getSymbolBoReqBinding(){
		return BindingBuilder.bind(getSymbolBoReqQueue()).to(topicExchange()).with(CommonConstants.GET_SYMBOLBO_REQ);
	}
	@Bean
	public Binding getTop5BoRemoveReqBinding(){
		return BindingBuilder.bind(getTop5BoRemoveReqQueue()).to(topicExchange()).with(CommonConstants.GET_TOP5BO_REMOVE_REQ);
	}
	@Bean
	public Binding getCustomBoRemoveReqBinding(){
		return BindingBuilder.bind(getCustomBoRemoveReqQueue()).to(topicExchange()).with(CommonConstants.GET_CUSTOMBO_REMOVE_REQ);
	}
	@Bean
	public Binding getSymbolBoRemoveReqBinding(){
		return BindingBuilder.bind(getSymbolBoRemoveReqQueue()).to(topicExchange()).with(CommonConstants.GET_SYMBOLBO_REMOVE_REQ);
	}
	@Bean
	public Binding cmTormComunicationBinding(){
		return BindingBuilder.bind(cmTormComunicationQueue()).to(topicExchange()).with(CommonConstants.RM_CM_COMMUNICATION_ERQ);
	}
		
	@Bean
	public Binding rmMonitorBinding(){
		return BindingBuilder.bind(rmMonitorQueue()).to(topicExchange()).with(CommonConstants.RM_MONITOR);
	}
	
	@Bean
	public SimpleMessageListenerContainer rmListenerMessageContainer() {
		SimpleMessageListenerContainer container = new SimpleMessageListenerContainer(connectionFactory());
		container.setQueues(getTop5BoReqQueue(),getCustomReqQueue(),getSymbolBoReqQueue(),getTop5BoRemoveReqQueue(),
				getCustomBoRemoveReqQueue(),getSymbolBoRemoveReqQueue(),cmTormComunicationQueue(),rmMonitorQueue());
		container.setExposeListenerChannel(true);
		container.setMaxConcurrentConsumers(1);
		container.setConcurrentConsumers(1);
		container.setAcknowledgeMode(AcknowledgeMode.NONE); 
		container.setMessageListener(receivedMsg);		
		return container;
	}
	/*@Bean
	public SimpleMessageListenerContainer orderMessageContainer() {
		SimpleMessageListenerContainer container = new SimpleMessageListenerContainer(connectionFactory());
		container.setQueues(orderAnsQueue());
		container.setExposeListenerChannel(true);
		container.setMaxConcurrentConsumers(1);
		container.setConcurrentConsumers(1);
		container.setAcknowledgeMode(AcknowledgeMode.NONE); 
		container.setMessageListener(orderRecv);		
		return container;
	}
	*/
}
