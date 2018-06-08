package com.broctagon.trad.rm.obj.spring;

import org.springframework.amqp.core.AcknowledgeMode;
import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.BindingBuilder;
import org.springframework.amqp.core.Queue;
import org.springframework.amqp.core.TopicExchange;
import org.springframework.amqp.rabbit.connection.CachingConnectionFactory;
import org.springframework.amqp.rabbit.connection.ConnectionFactory;
import org.springframework.amqp.rabbit.core.RabbitAdmin;
import org.springframework.amqp.rabbit.listener.SimpleMessageListenerContainer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.env.Environment;

import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.rs.rabbitmq.VerifierMessageRecv;


@Configuration
public class RabbitVarifierConfig {

	@Autowired
	private Environment environment;
	@Autowired
	private VerifierMessageRecv verifierMessageRecv;

	@Bean
	public ConnectionFactory connectionFactory() {
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
	public Queue getOrderAnsQueue() {
		return new Queue(CommonConstants.MQ_QUEUE_RM_GET_ORDER_ANS, false, false, true);
	}

	@Bean
	public Queue updateOrderAnsQueue() {
		return new Queue(CommonConstants.MQ_QUEUE_RM_UPDATE_ORDER_ANS, false, false, true);
	}

	@Bean
	public Queue updateOpenTradOnRootQueue() {
		return new Queue(CommonConstants.MQ_QUEUE_RM_UPDATE_OPENTRADE_ROOT, false, false, true);
	}

	@Bean
	public Queue updatecloseTradOnRootQueue() {
		return new Queue(CommonConstants.MQ_QUEUE_RM_UPDATE_CLOSETRADE_ROOT, false, false, true);
	}

	@Bean
	public Queue getPositionAnsQueue() {
		return new Queue(CommonConstants.MQ_QUEUE_RM_GETPOSITION_ANS, false, false, true);
	}

	@Bean
	public TopicExchange topicExchange() {
		return new TopicExchange(environment.getProperty("mq.exchange"), false, false);
	}

	@Bean
	public Binding getOrderAnsBinding() {
		return BindingBuilder.bind(getOrderAnsQueue()).to(topicExchange())
				.with(CommonConstants.MQ_KEY_RM_GET_ORDER_ANS);
	}

	@Bean
	public Binding updateOrderAnsBinding() {
		return BindingBuilder.bind(updateOrderAnsQueue()).to(topicExchange())
				.with(CommonConstants.MQ_KEY_RM_UPDATE_ORDER_ANS);
	}

	@Bean
	public Binding updateOpenTradOnRootBinding() {
		return BindingBuilder.bind(updateOpenTradOnRootQueue()).to(topicExchange())
				.with(CommonConstants.MQ_KEY_RM_UPDATE_OPENTRADE_ROOT);
	}

	@Bean
	public Binding updatecloseTradOnRootBinding() {
		return BindingBuilder.bind(updatecloseTradOnRootQueue()).to(topicExchange())
				.with(CommonConstants.MQ_KEY_RM_UPDATE_CLOSETRADE_ROOT);
	}

	@Bean
	public Binding getPositionAnsBinding() {
		return BindingBuilder.bind(getPositionAnsQueue()).to(topicExchange())
				.with(CommonConstants.MQ_QUEUE_RM_GETPOSITION_ANS);
	}

	@Bean
	public SimpleMessageListenerContainer getPositionReqMessageContainer() {
		SimpleMessageListenerContainer container = new SimpleMessageListenerContainer(connectionFactory());
		container.setQueues(getOrderAnsQueue(), updateOrderAnsQueue(), updateOpenTradOnRootQueue(),
				updatecloseTradOnRootQueue(), getPositionAnsQueue());
		container.setExposeListenerChannel(true);
		container.setMaxConcurrentConsumers(1);
		container.setConcurrentConsumers(1);
		container.setAcknowledgeMode(AcknowledgeMode.NONE);
		container.setMessageListener(verifierMessageRecv);
		return container;
	}

}
