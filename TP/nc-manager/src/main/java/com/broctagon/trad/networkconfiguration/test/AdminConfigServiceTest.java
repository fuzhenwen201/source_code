package com.broctagon.trad.networkconfiguration.test;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.trad.networkconfiguration.rabbitmq.RabbitMQHelper;

@Component
public class AdminConfigServiceTest {
		
	@Autowired
	private RabbitMQHelper rabbitMQHelper;
	
	
//	@PostConstruct
	public void testMqNotice(){
		rabbitMQHelper.publishChangeNoticeGroupBo("".getBytes());
		rabbitMQHelper.publishChangeNoticeGroupFx("".getBytes());
		rabbitMQHelper.publishChangeNoticeSecurityBo("".getBytes());
		rabbitMQHelper.publishChangeNoticeSecurityFx("".getBytes());
	}
	
	
	
}
