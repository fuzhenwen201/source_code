package cn.fintechstar.dcb.config;

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

import cn.fintechstar.dcb.constant.RabbitProperties;


@Configuration
public class RabbitConfig {

	@Autowired
	private RabbitProperties rabbitProperties;

	@Bean
	@Scope(ConfigurableBeanFactory.SCOPE_SINGLETON)
	public ConnectionFactory rabbitConnectionFactory() {
		CachingConnectionFactory connectionFactory = new CachingConnectionFactory();
		connectionFactory.setAddresses(rabbitProperties.getHost());
		connectionFactory.setPort(rabbitProperties.getPort());
		connectionFactory.setVirtualHost(rabbitProperties.getVhost());
		connectionFactory.setUsername(rabbitProperties.getUsername());
		connectionFactory.setPassword(rabbitProperties.getPassword());
		connectionFactory.setRequestedHeartBeat(0);
		connectionFactory.setChannelCacheSize(100);
		connectionFactory.setPublisherConfirms(false);
		connectionFactory.setPublisherReturns(false);
		return connectionFactory;
	}

	@Bean
	public TopicExchange nodeExchange() {
		return new TopicExchange(rabbitProperties.getInstantExchange(), false, false);
	}

	@Bean
	public Queue instantSymbolQueue(){
		return new Queue(rabbitProperties.getInstantSignalQueue().get("queueName"), false, false, true);
	}
	
	@Bean
	public Binding historySignalReqBinding() {
		return BindingBuilder.bind(instantSymbolQueue()).to(nodeExchange()).with(rabbitProperties.getInstantSignalQueue().get("routingKey"));
	}
	
	@Bean
	public RabbitAdmin rabbitAdmin() {
		RabbitAdmin rabbitAdmin = new RabbitAdmin(rabbitConnectionFactory());
		rabbitAdmin.declareQueue();
		rabbitAdmin.setAutoStartup(true);
		return rabbitAdmin;
	}

	@Bean
	public RabbitTemplate rabbitTemplate() {
		return rabbitAdmin().getRabbitTemplate();
	}

}
