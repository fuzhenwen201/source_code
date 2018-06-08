package com.broctagon.centrallogging.config;

import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.BindingBuilder;
import org.springframework.amqp.core.Queue;
import org.springframework.amqp.core.TopicExchange;
import org.springframework.amqp.rabbit.connection.CachingConnectionFactory;
import org.springframework.amqp.rabbit.connection.ConnectionFactory;
import org.springframework.amqp.rabbit.core.RabbitAdmin;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.config.ConfigurableBeanFactory;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Scope;

import com.broctagon.centrallogging.prop.MQProperties;


@Configuration
public class RabbitConfig {
	
	@Autowired
	private MQProperties mqProperties;

	@Bean
	public ConnectionFactory connectionFactory(){
		CachingConnectionFactory connectionFactory = new CachingConnectionFactory();
		connectionFactory.setAddresses(mqProperties.getHost());
		connectionFactory.setPort(mqProperties.getPort());
		connectionFactory.setVirtualHost(mqProperties.getVhost());
		connectionFactory.setUsername(mqProperties.getUsername());
		connectionFactory.setPassword(mqProperties.getPassword());
		connectionFactory.setRequestedHeartBeat(mqProperties.getErrorHeartBeats());
		connectionFactory.setChannelCacheSize(100);
		connectionFactory.setPublisherConfirms(false);
		connectionFactory.setPublisherReturns(false);
		return connectionFactory;
	}
	
	@Bean
	public RabbitAdmin rabbitAdmin(){
		RabbitAdmin rabbitAdmin = new RabbitAdmin(connectionFactory());
		rabbitAdmin.setAutoStartup(true);
		rabbitAdmin.declareQueue();
		return rabbitAdmin;
	}

	@Bean
	@Scope(ConfigurableBeanFactory.SCOPE_PROTOTYPE)
	public RabbitTemplate rabbitTemplate(){
		return rabbitAdmin().getRabbitTemplate();
	}
	
	@Bean
	public Queue errorQueue(){
		return new Queue(mqProperties.getErrorQueue(), false, false, true);
	}
	
	@Bean
	public Queue heartBeatQueue(){
		return new Queue(mqProperties.getHeartBeatQueue(), false, false, true);
	}
	
	@Bean
	public Queue controlQueue(){
		return new Queue(mqProperties.getControlQueue(), false, false, true);
	}
	
	@Bean
	public TopicExchange topicExchange(){
		return new TopicExchange(mqProperties.getExchange(), false, false);
	}
	
	@Bean
	public Binding errorBinding(){
		return BindingBuilder.bind(errorQueue()).to(topicExchange()).with(mqProperties.getErrorRoutingKey());
	}
	
	@Bean
	public Binding heartBeatBinding(){
		return BindingBuilder.bind(heartBeatQueue()).to(topicExchange()).with(mqProperties.getHeartBeatRoutingKey());
	}
	
	@Bean
	public Binding heartBeatBaseBinding(){
		return BindingBuilder.bind(heartBeatQueue()).to(topicExchange()).with(mqProperties.getHeartBeatRoutingKeyBase());
	}
	
	@Bean
	public Binding controlBinding(){
		return BindingBuilder.bind(controlQueue()).to(topicExchange()).with(mqProperties.getControlRoutingKey());
	}
}
