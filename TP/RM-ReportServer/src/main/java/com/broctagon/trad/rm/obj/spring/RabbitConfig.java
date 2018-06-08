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
import com.broctagon.trad.rm.obj.rs.rabbitmq.GetPositionsReqRecv;
import com.broctagon.trad.rm.obj.rs.rabbitmq.UpdateTraderRecv;


@Configuration
public class RabbitConfig {

	@Autowired
	private Environment environment;
	@Autowired
	private GetPositionsReqRecv getPositionsReqRecv;
	@Autowired
	private UpdateTraderRecv updateTraderRecv;

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
	@Scope(ConfigurableBeanFactory.SCOPE_PROTOTYPE)
	public RabbitTemplate rabbitSender() {
		RabbitTemplate rabbitTemplate = rabbitAdmin().getRabbitTemplate();
		rabbitTemplate.setExchange(environment.getProperty("mq.exchange"));
		rabbitTemplate.setRoutingKey(CommonConstants.MQ_KEY_RM_GET_ORDER_ANS);
		return rabbitTemplate;
	}

	@Bean
	public Queue orderReqQueue() {
		return new Queue(CommonConstants.MQ_KEY_RM_GET_ORDER_REQ, false, false, true);
	}

	@Bean
	public Queue updateTradBalanceQueue() {
		return new Queue(CommonConstants.MQ_QUEUE_RM_UPDATE_TRADE_BALANCE, false, false, true);
	}

	@Bean
	public Queue getPositionReqQueue() {
		return new Queue(CommonConstants.MQ_QUEUE_RM_GETPOSITION_REQ, false, false, true);
	}

	@Bean
	public Queue cmToRMTradersQueue() {
		return new Queue(CommonConstants.MQ_QUEUE_CM_RM_GET_TRADERS, false, false, true);
	}

	@Bean
	public TopicExchange topicExchange() {
		return new TopicExchange(environment.getProperty("mq.exchange"), false, false);
	}

	@Bean
	public Binding orderReqQueueBinding() {
		return BindingBuilder.bind(orderReqQueue()).to(topicExchange()).with(CommonConstants.MQ_KEY_RM_GET_ORDER_REQ);
	}

	@Bean
	public Binding updateTradBalanceBinding() {
		return BindingBuilder.bind(updateTradBalanceQueue()).to(topicExchange())
				.with(CommonConstants.MQ_KEY_RM_UPDATE_TRADE_BALANCE);
	}

	@Bean
	public Binding ugetPositionReqBinding() {
		return BindingBuilder.bind(getPositionReqQueue()).to(topicExchange())
				.with(CommonConstants.MQ_KEY_RM_GETPOSITION_REQ);
	}

	@Bean
	public Binding CMToRMTradersBinding() {
		return BindingBuilder.bind(cmToRMTradersQueue()).to(topicExchange())
				.with(CommonConstants.MQ_KEY_CM_RM_GET_TRADERS);
	}

	@Bean
	public SimpleMessageListenerContainer getPositionReqMessageContainer() {
		SimpleMessageListenerContainer container = new SimpleMessageListenerContainer(connectionFactory());
		container.setQueues(orderReqQueue(), getPositionReqQueue());
		container.setExposeListenerChannel(true);
		container.setMaxConcurrentConsumers(1);
		container.setConcurrentConsumers(1);
		container.setAcknowledgeMode(AcknowledgeMode.NONE);
		container.setMessageListener(getPositionsReqRecv);
		return container;
	}

	@Bean
	public SimpleMessageListenerContainer updateTradeRecvMessageContainer() {
		SimpleMessageListenerContainer container = new SimpleMessageListenerContainer(connectionFactory());
		container.setQueues(updateTradBalanceQueue(), cmToRMTradersQueue());
		container.setExposeListenerChannel(true);
		container.setMaxConcurrentConsumers(1);
		container.setConcurrentConsumers(1);
		container.setAcknowledgeMode(AcknowledgeMode.NONE);
		container.setMessageListener(updateTraderRecv);
		return container;
	}

}
