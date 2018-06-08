package com.broctagon.trad.networkconfiguration.test;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.trad.networkconfiguration.common.CommonConstants;
import com.broctagon.trad.networkconfiguration.rabbitmq.RabbitMQHelper;

@Component
public class RabbitMQTest {
	
	@Autowired
	private RabbitMQHelper rabbitMQHelper;
	
//	@PostConstruct
	public void testAllSending(){
		rabbitMQHelper.publishBoSymbolChangeNotification(CommonConstants.BOSYMBOL_CHANGE.getBytes());
		rabbitMQHelper.publishGroupSpreadChangeNotification(CommonConstants.GROUP_SPREAD_CHANGE.getBytes());
		rabbitMQHelper.publishSymbolSpreadChangeNotification(CommonConstants.SYMBOL_SPREAD_CHANGE.getBytes());
		rabbitMQHelper.publishUrlMapChangeNotification(CommonConstants.URL_MAP_CHANGE.getBytes());
		
		rabbitMQHelper.publishGroupSpreadChangeNotification_Old(CommonConstants.GROUP_SPREAD_CHANGE.getBytes());
		rabbitMQHelper.publishBoSymbolChangeNotification_Old(CommonConstants.BOSYMBOL_CHANGE.getBytes());
	}

}
