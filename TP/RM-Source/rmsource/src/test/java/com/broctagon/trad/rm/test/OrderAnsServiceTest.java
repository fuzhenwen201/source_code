/**
 * 
 */
package com.broctagon.trad.rm.test;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import com.broctagon.trad.rm.src.SpringBootApp;
import com.broctagon.trad.rm.src.service.OrderAnsService;

/**
 * @author imdadullah
 *
 */
@RunWith(SpringRunner.class)
@SpringBootTest(classes=SpringBootApp.class)
public class OrderAnsServiceTest {

	@Autowired
	private OrderAnsService orderAnsService;
	
	@Test
	public void orderAnsTest(){
		orderAnsService.storeOrderAns("".getBytes());
		
	}
}