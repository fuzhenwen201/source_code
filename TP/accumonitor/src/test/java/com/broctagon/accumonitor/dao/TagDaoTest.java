package com.broctagon.accumonitor.dao;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.accumonitor.constant.Tag;
import com.broctagon.tradtnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradtnode.rabbit.message.Message;

//@RunWith(SpringJUnit4ClassRunner.class)
//@SpringBootTest
public class TagDaoTest {

	@Autowired
	private TagDao tagDao;
	
//	@Test
	public void testAddTagError(){
		Message message = new Message();
		message.setMessageString("This is a test message");
		message.setMessageType(TNodeChannelMsgType.TNODE_EVENT_MSG_TYPE);
		message.setResponceQueue("queuename");
		message.setTag(Tag.LOGIN_REQ);
		tagDao.addTagError(message);
	}
}
