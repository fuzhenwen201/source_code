package com.broctagon.accumonitor.dao;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.accumonitor.dao.LogoutDao;

//@RunWith(SpringJUnit4ClassRunner.class)
//@SpringBootTest
public class LogoutDaoTest {

	@Autowired
	private LogoutDao logoutDao;
	
//	@Test
	public void addLogoutReqTest(){
		logoutDao.addLogoutReq("reqid", "request");
	}
	
//	@Test
	public void addLogoutAnsTest(){
		logoutDao.addLogoutAns("reqid", "response");
	}
	
//	@Test
	public void detelteLogoutByRequestIdTest(){
		logoutDao.detelteLogoutByRequestId("'cf6110b543a1460291e1394c56b817a2', '04de6630534545738867747e3a669c95'");
	}
}
