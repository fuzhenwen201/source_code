package cn.fintechstar.admin.manager;

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
import cn.fintechstar.admin.dao.manager.IManagerRightDao;
import cn.fintechstar.admin.model.manager.AccessRight;
import cn.fintechstar.admin.model.manager.Manager;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class ManagerRightDaoTest {

	@Autowired
	private IManagerRightDao  managerRightDao;
	
	@Autowired
	private IManagerDao managerDao;
	
	private static AccessRight accessRight;
	
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
		
		accessRight = new AccessRight();
		accessRight.setCreateUser(1);
		accessRight.setDeleteTrader(1);
		accessRight.setUpdateTrader(1);
		accessRight.setViewTrader(1);
		accessRight.setViewTraderDetails(1);
		accessRight.setCreateManager(1);
		accessRight.setDeleteManager(1);
		accessRight.setUpdateManager(1);
		accessRight.setViewManager(1);
		accessRight.setViewManagerDetails(1);
		accessRight.setModifyManagerAccess(1);
		accessRight.setCredit(1);
		accessRight.setBalance(1);
		accessRight.setBoMonitoring(1);
	}
	
	@Test
	@Rollback
	@Transactional
	public void managerRightDaoTest(){
		managerDao.saveManager(manager);
		accessRight.setManagerId(manager.getManagerId());
		managerRightDao.saveManagerAccessRight(accessRight);
		AccessRight accessRightFromDB = managerRightDao.getManagerAccessRightByManagerId(manager.getManagerId());
		Assert.assertEquals(accessRight.toString(), accessRightFromDB.toString());
		
		managerRightDao.deleteManagerAccessRightByManagerId(manager.getManagerId());
		accessRightFromDB = managerRightDao.getManagerAccessRightByManagerId(manager.getManagerId());
		Assert.assertNull(accessRightFromDB);
	}
}
