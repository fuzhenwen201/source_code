package com.broctagon.trad.rm.src.spring;

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

import com.broctagon.trad.rm.src.common.CommonConstants;
import com.broctagon.trad.rm.src.rabbitmq.OrderRecv;
import com.broctagon.trad.rm.src.rabbitmq.UserInfoRecv;


@Configuration
public class RabbitConfig {

	@Autowired private Environment environment;
	@Autowired private UserInfoRecv userInfoRecv;
	@Autowired private OrderRecv orderRecv;
	
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
	public RabbitTemplate userInfoSender() {
		RabbitTemplate rabbitTemplate = rabbitAdmin().getRabbitTemplate();
		rabbitTemplate.setExchange(environment.getProperty("mq.exchange"));
		rabbitTemplate.setRoutingKey(CommonConstants.MQ_BINDINGKEY_USERINFO_RM);
		return rabbitTemplate;
	}
	@Bean
	@Scope(ConfigurableBeanFactory.SCOPE_PROTOTYPE)
	public RabbitTemplate positionSender() {
		RabbitTemplate rabbitTemplate = rabbitAdmin().getRabbitTemplate();
		rabbitTemplate.setExchange(environment.getProperty("mq.exchange"));
		rabbitTemplate.setRoutingKey(CommonConstants.MQ_BINDINGKEY_ORDERANS_RM);
		return rabbitTemplate;
	}
	
	@Bean
	public Queue onlineBalanceQueue(){
		return new Queue(CommonConstants.MQ_QUEUE_USERONLINE_BALANCE, false, false, true);
	}
	@Bean
	public Queue offlineUserInfoQueue(){
		return new Queue(CommonConstants.MQ_QUEUE_OFFLINE_USERINFO_RM, false, false, true);
	}
	@Bean
	public Queue userInfoQueue(){
		return new Queue(CommonConstants.MQ_QUEUE_USERINFO_RM, false, false, true);
	}
	@Bean
	public Queue orderAnsQueue(){
		return new Queue(CommonConstants.MQ_QUEUE_ORDERANS_RM, false, false, true);
	}
	
	@Bean
	public TopicExchange topicExchange(){
		return new TopicExchange(environment.getProperty("mq.exchange"), false, false);
	}
	
	@Bean
	public Binding onlineBalanceBinding(){
		return BindingBuilder.bind(onlineBalanceQueue()).to(topicExchange()).with(CommonConstants.MQ_KEY_USERONLINE_BALANCE);
	}	
	@Bean
	public Binding offlineUserInfoBinding(){
		return BindingBuilder.bind(offlineUserInfoQueue()).to(topicExchange()).with(CommonConstants.MQ_KEY_OFFLINE_USERINFO_RM);
	}
	@Bean
	public Binding userInfoBinding(){
		return BindingBuilder.bind(userInfoQueue()).to(topicExchange()).with(CommonConstants.MQ_BINDINGKEY_USERINFO_RM);
	}
	@Bean
	public Binding onlineUserAnsBinding(){
		return BindingBuilder.bind(userInfoQueue()).to(topicExchange()).with(CommonConstants.MQ_BINDINGKEY_GET_ONLINETRADER_ANS);
	}
	@Bean
	public Binding userLoginBinding(){
		return BindingBuilder.bind(userInfoQueue()).to(topicExchange()).with(CommonConstants.MQ_BINDINGKEY_TRADER_LOGIN_ANS);
	}
	@Bean
	public Binding userLogoutBinding(){
		return BindingBuilder.bind(userInfoQueue()).to(topicExchange()).with(CommonConstants.MQ_BINDINGKEY_TRADER_LOGOUT_ANS);
	}
	@Bean
	public Binding userForceLogoutBinding(){
		return BindingBuilder.bind(userInfoQueue()).to(topicExchange()).with(CommonConstants.MQ_BINDINGKEY_TRADER_FORCELOGOUT_ANS);
	}
		
	@Bean
	public Binding orderAnsBinding(){
		return BindingBuilder.bind(orderAnsQueue()).to(topicExchange()).with(CommonConstants.MQ_BINDINGKEY_ORDERANS_RM);
	}
	
	@Bean
	public SimpleMessageListenerContainer userinfoMessageContainer() {
		SimpleMessageListenerContainer container = new SimpleMessageListenerContainer(connectionFactory());
		container.setQueues(onlineBalanceQueue(),offlineUserInfoQueue(),userInfoQueue());
		container.setExposeListenerChannel(true);
		container.setMaxConcurrentConsumers(1);
		container.setConcurrentConsumers(1);
		container.setAcknowledgeMode(AcknowledgeMode.NONE); 
		container.setMessageListener(userInfoRecv);		
		return container;
	}
	@Bean
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
	
}
