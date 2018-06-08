package com.broctagon.centrallogging;

import org.assertj.core.api.Assertions;
import org.junit.Before;
import org.junit.Test;
import org.junit.rules.ExpectedException;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.centrallogging.constant.Tag;
import com.broctagon.centrallogging.model.Message;
import com.broctagon.centrallogging.send.MailSender;

//@RunWith(SpringJUnit4ClassRunner.class)
//@SpringBootTest(classes=App.class)
public class MailTest {
	
	@Autowired
	private MailSender mailSender;
	
	private Message message;
	
	@Before
	public void init(){
		message = new Message();
		message.setReqid("2c2f4841c9fa49eb961ca32a3cff9110");
		message.setSessionid("e0bbd01e3a0b11e7bf8e000c29065d62");
		message.setGroupid(0);
		message.setClientid(72);
		message.setAccountid(72);
		message.setBalance(0);
		message.setPrevbalance(0);
		message.setChanged(100000);
		message.setMargin(0);
		message.setFreemargin(99041);
		message.setProfit(-960);
		message.setType(1);
		message.setClosebalance(0);
		message.setStatus(4);
	}

//	@Test
	public void testGetSentTo(){
		String sendTo = null;
		ExpectedException thrown = ExpectedException.none();
		thrown.expect(RuntimeException.class);
		sendTo = "";
		thrown.expect(RuntimeException.class);
		sendTo = " ";
		thrown.expect(RuntimeException.class);
		
		sendTo = "111222333@qq.com";
		Assertions.assertThat(mailSender.getSendTo(sendTo).length).isEqualTo(1);
		sendTo = "111222333@qq.com;222333444@qq.com";
		Assertions.assertThat(mailSender.getSendTo(sendTo).length).isEqualTo(2);
		sendTo = "111222333@qq.com;222333444@qq.com";
		Assertions.assertThat(mailSender.getSendTo(sendTo).length).isEqualTo(2);
	}
	
//	@Test
	public void testSendEmail() throws Exception{
		mailSender.send(message, Tag.MONITOR_TAG);
		mailSender.send(message, Tag.SIGNAL_TAG);
	}
}
