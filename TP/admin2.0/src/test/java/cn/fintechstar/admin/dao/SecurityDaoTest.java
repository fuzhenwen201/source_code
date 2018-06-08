package cn.fintechstar.admin.dao;

import java.util.Date;
import java.util.List;

import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.annotation.Rollback;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.security.Security;
import cn.fintechstar.admin.model.synch.SecurityOfSymbol;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class SecurityDaoTest {

	@Autowired
	private ISecurityDao securityDao;
	
	@Test
	@Rollback
	@Transactional
	public void addSecurityTest(){
		Security security = new Security();
		security.setSecurityName("security name");
		security.setDescription("create security test");
		security.setIsDeleted(0);
		security.setIsEnable(1);
		security.setType("bo");
		securityDao.saveSecurity(security);
		Security securityFromDB = securityDao.getSecurityById(security.getSecurityId());
		Assert.assertEquals(securityFromDB.getSecurityName(), "security name");
		Assert.assertEquals(securityFromDB.getType(), "bo");
		Assert.assertEquals(securityFromDB.getDescription(), "create security test");
	}
	
	@Test
	@Rollback
	@Transactional
	public void updateSecurityTest(){
		Security security = new Security();
		security.setSecurityName("security name");
		security.setDescription("create security test");
		security.setIsDeleted(0);
		security.setIsEnable(1);
		security.setType("fx");
		security.setCreatedTime(new Date());
		security.setModifiedTime(new Date());
		securityDao.saveSecurity(security);
		security.setDescription("update security test");
		securityDao.updateSecurity(security);
		Security securityFromDB = securityDao.getSecurityById(security.getSecurityId());
		Assert.assertEquals(securityFromDB.getDescription(), "update security test");
	}
	
	@Test
	@Rollback
	@Transactional
	public void deleteSecurityByIdTest(){
		Security security = new Security();
		security.setSecurityName("security name");
		security.setDescription("create security test");
		security.setIsDeleted(0);
		security.setIsEnable(1);
		security.setType("bo");
		security.setCreatedTime(new Date());
		security.setModifiedTime(new Date());
		securityDao.saveSecurity(security);
		securityDao.deleteSecurityById(security.getSecurityId());
		Security securityFromDB = securityDao.getSecurityById(security.getSecurityId());
		Assert.assertNull(securityFromDB);
	}
	
	@Test
	@Rollback
	@Transactional
	public void getBoSecuritiesOfGroupTest(){
		Security security = new Security();
		security.setSecurityName("security name");
		security.setDescription("create security test");
		security.setIsDeleted(0);
		security.setIsEnable(1);
		security.setType("bo");
		security.setCreatedTime(new Date());
		security.setModifiedTime(new Date());
		securityDao.saveSecurity(security);
		List<SecurityOfSymbol> securityOfSymbols = securityDao.getBoSecuritiesOfGroup();
		Assert.assertTrue(securityOfSymbols.size() >= 1);
	}
	
	@Test
	@Rollback
	@Transactional
	public void getFxSecuritiesOfGroupTest(){
		Security security = new Security();
		security.setSecurityName("security name");
		security.setDescription("create security test");
		security.setIsDeleted(0);
		security.setIsEnable(1);
		security.setType("fx");
		security.setCreatedTime(new Date());
		security.setModifiedTime(new Date());
		securityDao.saveSecurity(security);
		List<SecurityOfSymbol> securityOfSymbols = securityDao.getFxSecuritiesOfGroup();
		Assert.assertTrue(securityOfSymbols.size() >= 1);
	}
}
