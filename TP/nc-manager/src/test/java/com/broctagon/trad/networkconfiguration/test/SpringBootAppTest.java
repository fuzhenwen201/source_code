
package com.broctagon.trad.networkconfiguration.test;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.trad.networkconfiguration.SpringBootApp;

/**
* @auther: Water
* @time: Oct 13, 2017 6:51:29 PM
* 
*/

//@RunWith(SpringJUnit4ClassRunner.class)
//@SpringBootTest(classes=SpringBootApp.class)
public class SpringBootAppTest {

	@Autowired
	private AdminConfigServiceTest adminConfigServiceTest;
	
	
//	@Test
	public void testMqNotice(){
		adminConfigServiceTest.testMqNotice();
	}
	
}
