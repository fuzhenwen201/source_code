package cn.fintechstar.receivetest.config;

import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.BindingBuilder;
import org.springframework.amqp.core.Queue;
import org.springframework.amqp.core.TopicExchange;
import org.springframework.amqp.rabbit.connection.CachingConnectionFactory;
import org.springframework.amqp.rabbit.connection.ConnectionFactory;
import org.springframework.amqp.rabbit.core.RabbitAdmin;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Scope;

import cn.fintechstar.receivetest.prop.RabbitProp;
	

@Configuration
public class RabbitConfig {
	
	@Autowired
	private RabbitProp rabbitProp;
	
	@Bean
	public ConnectionFactory connectionFactory(){
		CachingConnectionFactory  connectionFactory = new CachingConnectionFactory();
		connectionFactory.setAddresses(rabbitProp.getHost());
		connectionFactory.setPort(rabbitProp.getPort());
		connectionFactory.setVirtualHost(rabbitProp.getVhost());
		connectionFactory.setUsername(rabbitProp.getUsername());
		connectionFactory.setPassword(rabbitProp.getPassword());
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
	@Scope
	public RabbitTemplate rabbitTemplate(){
		return rabbitAdmin().getRabbitTemplate();
	}
	
	@Bean
	public TopicExchange topicExchange(){
		return new TopicExchange(rabbitProp.getExchange(), false, false);
	}
	
//	@Bean
//	public Queue reqQueue(){
//		return new Queue(rabbitProp.getReqQueue(), false, false, true);
//	}
//	
//	@Bean
//	public Binding reqBinding(){
//		return BindingBuilder.bind(reqQueue()).to(topicExchange()).with(rabbitProp.getReqRoutingKey());
//	}

	@Bean
	public Queue ansQueue(){
		return new Queue(rabbitProp.getAnsQueue(), false, false, true);
	}
	
	@Bean
	public Binding ansBinding(){
		return BindingBuilder.bind(ansQueue()).to(topicExchange()).with(rabbitProp.getAnsRoutingKey());
	}
}
