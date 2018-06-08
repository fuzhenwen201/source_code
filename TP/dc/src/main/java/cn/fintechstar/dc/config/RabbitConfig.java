package cn.fintechstar.dc.config;

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

import cn.fintechstar.dc.constant.RabbitProperties;


@Configuration
public class RabbitConfig {

	@Autowired
	private RabbitProperties rabbitProperties;

	@Bean
	public ConnectionFactory connectionFactory(){
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
	public TopicExchange signalExchange() {
		return new TopicExchange(rabbitProperties.getSignalExchange(), false, false);
	}

	@Bean
	public Queue historySignalReqQueue() {
		return new Queue(rabbitProperties.getQueue().get("historySignalReq"), false, false, true);
	}

	@Bean
	public Binding historySignalReqBinding() {
		return BindingBuilder.bind(historySignalReqQueue()).to(signalExchange()).with(rabbitProperties.getQueue().get("historySignalReq"));
	}

}
