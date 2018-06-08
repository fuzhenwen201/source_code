package cn.fintechstar.admin.config.rabbit;

import org.springframework.amqp.rabbit.connection.CachingConnectionFactory;
import org.springframework.amqp.rabbit.connection.ConnectionFactory;
import org.springframework.amqp.rabbit.core.RabbitAdmin;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.config.ConfigurableBeanFactory;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Scope;
import org.springframework.core.env.Environment;
	

@Configuration
public class RabbitConfig {
	
	@Autowired
	private Environment env;
	
	@Bean
	@Scope(ConfigurableBeanFactory.SCOPE_SINGLETON)
	public ConnectionFactory connectionFactory(){
		CachingConnectionFactory connectionFactory = new CachingConnectionFactory();
		connectionFactory.setAddresses(env.getProperty("rabbit.host"));
		connectionFactory.setPort(Integer.parseInt(env.getProperty("rabbit.port")));
		connectionFactory.setVirtualHost(env.getProperty("rabbit.vhost"));
		connectionFactory.setUsername(env.getProperty("rabbit.username"));
		connectionFactory.setPassword(env.getProperty("rabbit.password"));
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
		RabbitTemplate rabbitTemplate = rabbitAdmin().getRabbitTemplate();
		rabbitTemplate.setExchange(env.getProperty("rabbit.adt.exchange"));
		return rabbitTemplate;
	}
	
}
