/**
 * 
 */
package com.broctagon.trad.rm.test.serviceimpl;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.obj.SpringBootApp;
import com.broctagon.trad.rm.obj.req.message.SymbolBoReqMsg;
import com.broctagon.trad.rm.obj.req.message.TopFiveBoReqMsg;
import com.broctagon.trad.rm.obj.res.message.CustomBoResMsg;
import com.broctagon.trad.rm.obj.service.BoReqService;
import com.broctagon.tradstnode.rabbit.message.Message;

/**
 * @author imdadullah
 *
 */
@RunWith(SpringRunner.class)
@SpringBootTest(classes=SpringBootApp.class)
public class BoReqServiceTest {
	@Autowired
	private BoReqService boReqService;
	
	@Test
	public void reqForTopFiveBoUp(){
		TopFiveBoReqMsg msg = new TopFiveBoReqMsg();
		msg.setDirection(1);
		msg.setManagerid(1);
		msg.setReqid("123");
		msg.setSession("456");
		msg.setSgid(2);
		String message = JSON.toJSONString(msg);
		boReqService.addTopFive5BoReq(message);
	}
	@Test
	public void reqForTopFiveBoDwn(){
		TopFiveBoReqMsg msg = new TopFiveBoReqMsg();
		msg.setDirection(0);
		msg.setManagerid(1);
		msg.setReqid("123");
		msg.setSession("456");
		msg.setSgid(2);
		String message = JSON.toJSONString(msg);
		boReqService.addTopFive5BoReq(message);
		}
	@Test
	public void reqForCustomUp(){
		CustomBoResMsg msg = new CustomBoResMsg();
		msg.setDirection(1);
		msg.setManagerid(1);
		msg.setReqid("123");
		String message = JSON.toJSONString(msg);
		boReqService.addCustomBoReq(message);
		
	}
	@Test
	public void reqForCustomDwn(){
		CustomBoResMsg msg = new CustomBoResMsg();
		msg.setDirection(0);
		msg.setManagerid(1);
		msg.setReqid("123");
		String message = JSON.toJSONString(msg);
		boReqService.addCustomBoReq(message);
		}
	@Test
	public void reqForSymbolBo(){
		SymbolBoReqMsg msg = new SymbolBoReqMsg();
		msg.setDirection(1);
		msg.setManagerid(1);
		msg.setReqid("123");
		msg.setSgid(12);
		String message = JSON.toJSONString(msg);
		Message ansMessage = new Message();
		ansMessage.setMessageString(message);
		boReqService.addSymbolBoReq(ansMessage);
	}
}
