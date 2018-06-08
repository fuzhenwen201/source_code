package com.broctagon.accumonitor.dao;

import java.util.List;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

//@RunWith(SpringJUnit4ClassRunner.class)
//@SpringBootTest
public class LoginDaoTest {

	@Autowired
	private LoginDao dao;
	
//	@Test
	public void testAddLoginReq(){
		dao.addLoginReq("test", "test");
	}
	
//	@Test
	public void testAddLoginAns(){
		dao.addLoginAns("test", "test");
	}
	
//	@Test
	public void testGetLoginInfoByRequestId(){
		List<String> logins = dao.getLoginInfoByRequestId("54b686308df341f0acc545d2a92c5b03");
		System.out.println(logins);
	}
	
//	@Test
	public void testDeleteMessageByRequestId(){
		dao.deleteMessageByRequestId("test");
	}
	
}
