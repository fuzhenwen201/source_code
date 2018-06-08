package com.broctagon.trad.networkconfiguration.spring;

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

import com.broctagon.trad.networkconfiguration.rabbitmq.MessageRecv;
import com.broctagon.trad.networkconfiguration.rabbitmq.RabbitConstants;


@Configuration
public class RabbitConfig {

	@Autowired private Environment environment;
	@Autowired private MessageRecv messageRecv;
	
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
	public RabbitTemplate rabbitTemplate() {
		RabbitTemplate rabbitTemplate = rabbitAdmin().getRabbitTemplate();
		rabbitTemplate.setExchange(RabbitConstants.MQ_EXCHANGE_NC);
		return rabbitTemplate;
	}
	
	@Bean
	public Queue verifierQueue(){
		System.err.println("verifierQueue");
		return new Queue(RabbitConstants.MQ_QUEUE_NC_FOLLOWER_VERIFIER_DEBUG, false, false, true);
	}
	
	@Bean
	public TopicExchange topicExchange(){
		return new TopicExchange(RabbitConstants.MQ_EXCHANGE_NC, false, false);
	}
	
	@Bean
	public TopicExchange topicExchange_Old(){
		return new TopicExchange(RabbitConstants.MQ_EXCHANGE_GRP, false, false);
	}
	
	@Bean
	public Binding symbolSpreadBinding(){
		return BindingBuilder.bind(verifierQueue()).to(topicExchange()).with(RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_SYMBOLSPREAD);
	}
	
	@Bean
	public Binding groupSpreadBinding(){
		return BindingBuilder.bind(verifierQueue()).to(topicExchange()).with(RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_GROUPSPREAD);
	}
	
	@Bean
	public Binding urlMapBinding(){
		return BindingBuilder.bind(verifierQueue()).to(topicExchange()).with(RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_URLMAP);
	}
	
	@Bean
	public Binding boSymbolBinding(){
		return BindingBuilder.bind(verifierQueue()).to(topicExchange()).with(RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_BOSYMBOL);
	}
	
	@Bean
	public Binding groupSpreadBinding_Old(){
		return BindingBuilder.bind(verifierQueue()).to(topicExchange_Old()).with(RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_GROUPSPREAD_OLD);
	}
	
	@Bean
	public Binding boSymbolBinding_Old(){
		return BindingBuilder.bind(verifierQueue()).to(topicExchange_Old()).with(RabbitConstants.MQ_KEY_NC_FOLLOWER_CHNAGENOTICE_BOSYMBOL_OLD);
	}
	
	@Bean
	public SimpleMessageListenerContainer messageContainer() {
		SimpleMessageListenerContainer container = new SimpleMessageListenerContainer(connectionFactory());
		container.setQueues(verifierQueue());
		container.setExposeListenerChannel(true);
		container.setMaxConcurrentConsumers(1);
		container.setConcurrentConsumers(1);
		container.setAcknowledgeMode(AcknowledgeMode.NONE); 
		container.setMessageListener(messageRecv);		
		return container;
	}
}
