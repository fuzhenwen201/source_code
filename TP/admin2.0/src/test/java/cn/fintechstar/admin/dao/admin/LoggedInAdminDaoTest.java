package cn.fintechstar.admin.dao.admin;

import java.util.Date;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.annotation.Rollback;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.admin.Admin;
import cn.fintechstar.admin.model.admin.LoggedInAdmin;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class LoggedInAdminDaoTest {
	
	@Autowired
	private IAdminDao adminDao;
	
	@Autowired
	private ILoggedInAdminDao loggedInAdminDao;
	
	private static Admin admin;
	
	private static LoggedInAdmin loggedInAdmin;
	
	@BeforeClass
	public static void init(){
		admin = new Admin();
		admin.setName("unit test");
		admin.setPassword("password");
		admin.setAddress("address");
		admin.setPhoneNo("110");
		admin.setIsDeleted(false);
		admin.setIsSuperAdmin(false);
		
		loggedInAdmin = new LoggedInAdmin();
		loggedInAdmin.setIpAddress("127.0.0.1");
		loggedInAdmin.setMacAddress("45-8F-5C-9C-A4-66");
		loggedInAdmin.setHardDiskSerialNo("H2N3ADFMA5544");
		loggedInAdmin.setReqId("2fhkah893hk344");
		loggedInAdmin.setSessionId("31b53b3ca27a11e7aaf8bda2475936c8");
		loggedInAdmin.setLoginTime(new Date());
		loggedInAdmin.setLogoutTime(new Date());
	}

	@Test
	@Rollback
	@Transactional
	public void loggedInAdminDaoTest(){
		// -- save --
		adminDao.saveAdmin(admin);
		loggedInAdmin.setAdminId(admin.getAdminId());
		loggedInAdminDao.saveLoggedInAdmin(loggedInAdmin);
		LoggedInAdmin loggedInAdminFromDB = loggedInAdminDao.getLoggedInAdminById(loggedInAdmin.getId());
		Assert.assertEquals(loggedInAdminFromDB.toString(), loggedInAdmin.toString());
		// -- update -- 
		loggedInAdmin.setLogoutTime(new Date());
		loggedInAdminDao.updateLoggedInAdmin(loggedInAdmin);
		loggedInAdminFromDB = loggedInAdminDao.getLoggedInAdminById(loggedInAdmin.getId());
		Assert.assertEquals(loggedInAdminFromDB.toString(), loggedInAdmin.toString());
	}
	
}
