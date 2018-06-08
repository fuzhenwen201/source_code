package com.broctagon.accumonitor.dao;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.accumonitor.dao.UserInfoDao;

//@RunWith(SpringJUnit4ClassRunner.class)
//@SpringBootTest
public class UserInfoDaoTest {

	@Autowired
	private UserInfoDao userDao;
	
//	@Test
	public void testAddUser(){
		userDao.addUserInfo("test2", "userinfo");
	}
	
}
