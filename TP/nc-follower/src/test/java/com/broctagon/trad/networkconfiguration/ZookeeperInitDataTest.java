package com.broctagon.trad.networkconfiguration;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.trad.networkconfiguration.common.CommonConstants;
import com.broctagon.trad.networkconfiguration.rabbitmq.RabbitMQHelper;
import com.broctagon.trad.networkconfiguration.zookeeper.ZookeeperHelper;

//@RunWith(SpringJUnit4ClassRunner.class)
//@SpringBootTest(classes=SpringBootApp.class)
public class ZookeeperInitDataTest {
	
	@Autowired
	private ZookeeperHelper zookeeperHelper;
	
	@Test
	public void testInitData(){
		
	//	zookeeperHelper.init();
		
		System.out.println("testAllSending end");
	}

}
