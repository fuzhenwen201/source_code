package cn.fintechstar.admin.manager;

import java.util.List;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.annotation.Rollback;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.dao.manager.IManagerDao;
import cn.fintechstar.admin.model.manager.Manager;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class ManagerDaoTest {

	@Autowired
	private IManagerDao managerDao;
	
	private static Manager manager;
	
	@BeforeClass
	public static void init(){
		manager = new Manager();
		manager.setPassword("password");
		manager.setFirstName("firstName");
		manager.setLastName("lastName");
		manager.setEmail("email");
		manager.setAddress("address");
		manager.setCity("city");
		manager.setState("state");
		manager.setCountry("country");
		manager.setPin("pin");
		manager.setIsDeleted(false);
		manager.setIsEnable(true);
	}
	
	@Test
	@Rollback
	@Transactional
	public void managerDaoTest(){
		// -- save --
		managerDao.saveManager(manager);
		List<Manager> managerFromDB = managerDao.getManagerByManagerId(manager.getManagerId());
		Assert.assertEquals(managerFromDB.size(), 1);
		Assert.assertEquals(managerFromDB.get(0).toString(), manager.toString());
		// -- update --
		manager.setPassword("test");
		manager.setFirstName("test");
		manager.setLastName("test");
		manager.setEmail("test");
		manager.setAddress("test");
		manager.setCity("test");
		manager.setState("test");
		manager.setCountry("test");
		manager.setPin("test");
		manager.setIsDeleted(false);
		managerDao.updateManager(manager);
		managerFromDB = managerDao.getManagerByManagerId(manager.getManagerId());
		Assert.assertEquals(managerFromDB.size(), 1);
		Assert.assertEquals(managerFromDB.get(0).toString(), manager.toString());
		// -- enable manager --
		managerDao.enableManagerByManagerId(manager.getManagerId(), false);
		managerFromDB  = managerDao.getManagerByManagerId(manager.getManagerId());
		Assert.assertEquals(managerFromDB.size(), 1);
		Assert.assertFalse(managerFromDB.get(0).getIsEnable());
		// -- delete manager --
		managerDao.deleteManager(manager.getManagerId());
		managerFromDB  = managerDao.getManagerByManagerId(manager.getManagerId());
		Assert.assertEquals(managerFromDB.size(), 0);
	}
}
