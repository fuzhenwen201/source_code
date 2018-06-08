package com.broctagon.trad.networkconfiguration;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.trad.networkconfiguration.common.CommonConstants;
import com.broctagon.trad.networkconfiguration.rabbitmq.RabbitMQHelper;

//@RunWith(SpringJUnit4ClassRunner.class)
//@SpringBootTest(classes=SpringBootApp.class)
public class SpringBootAppTest {
	
	@Autowired
	private RabbitMQHelper rabbitMQHelper;
	
//	@Test
	public void testAllSending(){
		rabbitMQHelper.publishBoSymbolChangeNotification(CommonConstants.BOSYMBOL_CHANGE.getBytes());
		rabbitMQHelper.publishGroupSpreadChangeNotification(CommonConstants.GROUP_SPREAD_CHANGE.getBytes());
		rabbitMQHelper.publishSymbolSpreadChangeNotification(CommonConstants.SYMBOL_SPREAD_CHANGE.getBytes());
		rabbitMQHelper.publishUrlMapChangeNotification(CommonConstants.URL_MAP_CHANGE.getBytes());
		
		rabbitMQHelper.publishGroupSpreadChangeNotification_Old(CommonConstants.GROUP_SPREAD_CHANGE.getBytes());
		rabbitMQHelper.publishBoSymbolChangeNotification_Old(CommonConstants.BOSYMBOL_CHANGE.getBytes());
		
		try {
			Thread.sleep(5000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		System.out.println("testAllSending end");
	}

}
