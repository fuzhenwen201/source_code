package cn.fintechstar.admin.dao.admin;

import java.util.ArrayList;
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

import cn.fintechstar.admin.model.admin.Admin;
import cn.fintechstar.admin.model.admin.AdminVo;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class AdminDaoTest {
	
	@Autowired
	private IAdminDao adminDao;
	
	private static Admin admin;
	
	@BeforeClass
	public static void init(){
		admin = new Admin();
		admin.setName("unit test");
		admin.setPassword("password");
		admin.setAddress("address");
		admin.setPhoneNo("110");
		admin.setIsDeleted(false);
		admin.setIsSuperAdmin(false);
	}
	

	@Test
	@Rollback
	@Transactional
	public void adminDaoTest(){
		// -- save --
		adminDao.saveAdmin(admin);
		Admin adminFromDB = adminDao.getAdminByAdminId(admin.getAdminId());
		Assert.assertEquals(admin.toString(), adminFromDB.toString());
		// -- isSuperAdmin --
		int isSuperAdmin = adminDao.isSuperAdmin(admin.getAdminId());
		Assert.assertEquals(isSuperAdmin, 0);
		isSuperAdmin = adminDao.isSuperAdmin(1);
		Assert.assertEquals(isSuperAdmin, 1);
		// --  update --
		admin.setName("test");
		admin.setPassword("test");
		admin.setAddress("test");
		admin.setPhoneNo("120");
		admin.setIsDeleted(false);
		admin.setIsSuperAdmin(false);
		adminDao.updateAdmin(admin);
		adminFromDB = adminDao.getAdminByAdminId(admin.getAdminId());
		Assert.assertEquals(admin.toString(), adminFromDB.toString());
		// -- getAdmins --
		List<AdminVo> adminVos = adminDao.getAdmins();
		Assert.assertNotEquals(adminVos.size(), 0);
		// -- changePassword --
		adminDao.updateChangePasswordByAdminId(admin.getAdminId(), "hello");
		adminFromDB = adminDao.getAdminByAdminId(admin.getAdminId());
		Assert.assertEquals(adminFromDB.getPassword(), "hello");
		// -- delete --
		List<Integer> adminIds = new ArrayList<>();
		adminIds.add(admin.getAdminId());
		adminDao.deleteAdmins(adminIds);
		adminFromDB = adminDao.getAdminByAdminId(admin.getAdminId());
		System.err.println(adminFromDB);
	}
	
}
