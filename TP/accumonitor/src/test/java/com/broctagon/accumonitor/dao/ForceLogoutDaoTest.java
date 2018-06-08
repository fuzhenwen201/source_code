package com.broctagon.accumonitor.dao;

import java.util.List;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

//@RunWith(SpringJUnit4ClassRunner.class)
//@SpringBootTest
public class ForceLogoutDaoTest {

	@Autowired
	private ForceLogoutDao forceLogoutDao;
	
//	@Test
	public void addForceLogoutReqTest(){
		forceLogoutDao.addForceLogoutReq("test", "force logout request");
	}
	
//	@Test
	public void addForceLogoutAnsTest(){
		forceLogoutDao.addForceLogoutAns("test", "force logout response");
	}
	
//	@Test
	public void getRequestIdByEmptyForceLoginReqTest(){
		List<String> requestIds = forceLogoutDao.getRequestIdByEmptyForceLoginReq();
		System.out.println(requestIds);
	}
	
//	@Test
	public void addLoginInfoTest(){
		forceLogoutDao.addLoginInfo("8696f29247294dfeaefcb3ff47e7378e", "loginReq", "loginAns");
	}
}
