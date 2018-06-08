/**
 * 
 */
package com.broctagon.trad.rm.test.serviceimpl;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import com.broctagon.trad.rm.obj.SpringBootApp;
import com.broctagon.trad.rm.obj.common.CommonPublisher;

/**
 * @author imdadullah
 *
 */
@RunWith(SpringRunner.class)
@SpringBootTest(classes=SpringBootApp.class)
public class MqPublishTest {

	@Autowired
	private CommonPublisher commonPublisher;
	
	@Test
	public void testPublisher(){
		commonPublisher.publishTop5BoRes("", 2021);
		commonPublisher.publishTop5UpdatedBo("");
		commonPublisher.publishCustomizedBoRes("", 2021);
		commonPublisher.publishCustomizedUpdatedBo("");
		commonPublisher.publishSymbolBoRes("", 2021);
		commonPublisher.publishSymbolUpdatedBo("");
	}
}
