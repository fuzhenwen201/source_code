package com.broctagon.accumonitor.config;

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

import com.broctagon.accumonitor.prop.RabbitProp;
import com.broctagon.accumonitor.util.ConstantUtil;

@Configuration
public class RabbitConfig {

	@Autowired
	private RabbitProp rabbitProp;
	
	@Bean
	@Scope(ConfigurableBeanFactory.SCOPE_SINGLETON)
	public ConnectionFactory connectionFactory(){
		CachingConnectionFactory connectionFactory = new CachingConnectionFactory();
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
	@Scope(ConfigurableBeanFactory.SCOPE_PROTOTYPE)
	public RabbitTemplate rabbitTemplate(){
		return rabbitAdmin().getRabbitTemplate();
	}
	
	@Bean
	public TopicExchange topicExchange(){
		return new TopicExchange(rabbitProp.getExchange(), false, false);
	}
	
	@Bean 
	public Queue loginQueue(){
		return new Queue(rabbitProp.getQueue().get("login"), false, false, true);
	}
	
	@Bean
	public Binding logigBinding(){
		return BindingBuilder.bind(loginQueue()).to(topicExchange()).with(rabbitProp.getQueue().get("login"));
	}
	
	@Bean
	public Binding logigBinding2(){
		return BindingBuilder.bind(loginQueue()).to(topicExchange()).with(ConstantUtil.getRoutingKey(rabbitProp.getQueue().get("login")));
	}
	
	@Bean
	public Queue logoutQueue(){
		return new Queue(rabbitProp.getQueue().get("logout"), false, false, true);
	}
	
	@Bean
	public Binding logoutBinding(){
		return BindingBuilder.bind(logoutQueue()).to(topicExchange()).with(rabbitProp.getQueue().get("logout"));
	}
	
	@Bean
	public Binding logoutBinding2(){
		return BindingBuilder.bind(logoutQueue()).to(topicExchange()).with(ConstantUtil.getRoutingKey(rabbitProp.getQueue().get("logout")));
	}
	
	@Bean
	public Queue userInfoQueue(){
		return new Queue(rabbitProp.getQueue().get("userInfo"), false, false, true);
	}
	
	@Bean
	public Binding userInfoBinding(){
		return BindingBuilder.bind(userInfoQueue()).to(topicExchange()).with(rabbitProp.getQueue().get("userInfo"));
	}
	
	@Bean
	public Binding userInfoBinding2(){
		return BindingBuilder.bind(userInfoQueue()).to(topicExchange()).with(ConstantUtil.getRoutingKey(rabbitProp.getQueue().get("userInfo")));
	}
	
	@Bean
	public Queue ocpositionQueue(){
		return new Queue(rabbitProp.getQueue().get("ocposition"), false, false, true);
	}
	
	@Bean
	public Binding ocpositionBinding(){
		return BindingBuilder.bind(ocpositionQueue()).to(topicExchange()).with(rabbitProp.getQueue().get("ocposition"));
	}
	
	@Bean
	public Binding ocpositionBinding2(){
		return BindingBuilder.bind(ocpositionQueue()).to(topicExchange()).with(ConstantUtil.getRoutingKey(rabbitProp.getQueue().get("ocposition")));
	}
	
	@Bean
	public Queue pullQueue(){
		return new Queue(rabbitProp.getQueue().get("pull"), false, false, true);
	}
	
	@Bean
	public Binding pullBinding(){
		return BindingBuilder.bind(pullQueue()).to(topicExchange()).with(rabbitProp.getQueue().get("pull"));
	}
	
	@Bean
	public Binding pullBinding2(){
		return BindingBuilder.bind(pullQueue()).to(topicExchange()).with(ConstantUtil.getRoutingKey(rabbitProp.getQueue().get("pull")));
	}
	
	@Bean
	public Queue forcelogoutQueue(){
		return new Queue(rabbitProp.getQueue().get("forcelogout"), false, false, true);
	}
	
	@Bean
	public Binding forcelogoutBinding(){
		return BindingBuilder.bind(forcelogoutQueue()).to(topicExchange()).with(rabbitProp.getQueue().get("forcelogout"));
	}
	
	@Bean
	public Binding forcelogoutBinding2(){
		return BindingBuilder.bind(forcelogoutQueue()).to(topicExchange()).with(ConstantUtil.getRoutingKey(rabbitProp.getQueue().get("forcelogout")));
	}
}
