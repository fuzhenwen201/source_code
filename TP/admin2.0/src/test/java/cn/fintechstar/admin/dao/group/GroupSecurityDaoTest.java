package cn.fintechstar.admin.dao.group;

import java.util.ArrayList;
import java.util.Date;
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

import cn.fintechstar.admin.dao.ISecurityDao;
import cn.fintechstar.admin.model.group.GroupCommon;
import cn.fintechstar.admin.model.group.GroupSecurity;
import cn.fintechstar.admin.model.security.Security;
import cn.fintechstar.admin.model.synch.SecurityOfGroupBO;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class GroupSecurityDaoTest {

	@Autowired
	private IGroupDao groupDao;
	
	@Autowired
	private ISecurityDao securityDao;
	
	@Autowired
	private IGroupSecurityDao groupSecurityDao;
	
	private static GroupCommon groupCommon;
	
	private static Security securityCommon;
	
	private static Security securityCommon2;
	
	private static List<GroupSecurity> groupSecurities;
	
	@BeforeClass
	public static void init(){
		groupCommon = new GroupCommon();
		groupCommon.setGroupName("unit test");
		groupCommon.setIsEnable(1);
		groupCommon.setOtp("TOPT SHA-256");
		groupCommon.setIsEnable(1);
		groupCommon.setOwnerName("Broctagon Fintech Group");
		groupCommon.setSupportedPage("supported page");
		groupCommon.setDefaultDeposit(11.11);
		groupCommon.setDepositCurrency("USD");
		groupCommon.setDefaultLeverage(11.11);
		groupCommon.setAnnualInterest(11.11);
		groupCommon.setIsDeleted(0);
		groupCommon.setCreatedTime(new Date());
		groupCommon.setModifiedTime(new Date());
		
		securityCommon = new Security();
		securityCommon.setSecurityName("security name");
		securityCommon.setDescription("create security test");
		securityCommon.setIsDeleted(0);
		securityCommon.setIsEnable(1);
		securityCommon.setType("bo");
		securityCommon.setCreatedTime(new Date());
		securityCommon.setModifiedTime(new Date());
		
		securityCommon2 = new Security();
		securityCommon2.setSecurityName("security name");
		securityCommon2.setDescription("create security test");
		securityCommon2.setIsDeleted(0);
		securityCommon2.setIsEnable(1);
		securityCommon2.setType("fx");
		securityCommon2.setCreatedTime(new Date());
		securityCommon2.setModifiedTime(new Date());
		
		groupSecurities = new ArrayList<>();
		GroupSecurity groupSecurity = new GroupSecurity();
		groupSecurity.setIsInstruments(0);
		groupSecurity.setIsTrade(0);
		groupSecurity.setIsRequestMode(0);
		groupSecurity.setExecutionMode("execution mode");
		groupSecurity.setSpreadDiff(11.11);
		groupSecurity.setMaxDeviation(11.11);
		groupSecurity.setIsFreeMargin(0);
		groupSecurity.setIsFastConfirmation(0);
		groupSecurity.setIsCloseTwoOrder(0);
		groupSecurity.setIsCloseAllOrder(0);
		groupSecurity.setAutoClose("auto close");
		groupSecurity.setMinLot(10);
		groupSecurity.setMaxLot(100);
		groupSecurity.setStepSize(1);
		groupSecurity.setStdCommission(11.11);
		groupSecurity.setTaxCommission(11.11);
		groupSecurity.setAgentCommission(11.11);
		groupSecurity.setIsEnable(1);
		
		GroupSecurity groupSecurity2 = new GroupSecurity();
		groupSecurity2.setIsInstruments(0);
		groupSecurity2.setIsTrade(0);
		groupSecurity2.setIsRequestMode(0);
		groupSecurity2.setExecutionMode("execution mode");
		groupSecurity2.setSpreadDiff(11.11);
		groupSecurity2.setMaxDeviation(11.11);
		groupSecurity2.setIsFreeMargin(0);
		groupSecurity2.setIsFastConfirmation(0);
		groupSecurity2.setIsCloseTwoOrder(0);
		groupSecurity2.setIsCloseAllOrder(0);
		groupSecurity2.setAutoClose("auto close");
		groupSecurity2.setMinLot(10);
		groupSecurity2.setMaxLot(100);
		groupSecurity2.setStepSize(1);
		groupSecurity2.setStdCommission(11.11);
		groupSecurity2.setTaxCommission(11.11);
		groupSecurity2.setAgentCommission(11.11);
		groupSecurity2.setIsEnable(1);

		groupSecurities.add(groupSecurity);
		groupSecurities.add(groupSecurity2);
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateGroupSecurityTest(){
		// -- save --
		groupDao.saveGroup(groupCommon);
		securityDao.saveSecurity(securityCommon);
		securityDao.saveSecurity(securityCommon2);
		for(GroupSecurity groupSecurity : groupSecurities){
			groupSecurity.setGroupId(groupCommon.getGroupId());
		}
		groupSecurities.get(0).setSecurityId(securityCommon.getSecurityId());
		groupSecurities.get(1).setSecurityId(securityCommon2.getSecurityId());
		groupSecurityDao.saveGroupSecurities(groupSecurities);
		List<GroupSecurity> groupSecuritiesFromDB = groupSecurityDao.getGroupSecuritiesByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupSecurities.toString(), groupSecuritiesFromDB.toString());
		// -- getBoGroups --
		List<SecurityOfGroupBO> securityOfGroupBOs = groupSecurityDao.getBoGroups();
		Assert.assertTrue(securityOfGroupBOs.size() >= 1);
		// -- getFxGroups --
		List<SecurityOfGroupBO> securityOfGroupFxs = groupSecurityDao.getFxGroups();
		Assert.assertTrue(securityOfGroupFxs.size() >= 1);
		// -- delete --
		groupSecurityDao.deleteGroupSecuritiesByGroupId(groupCommon.getGroupId());
		groupSecuritiesFromDB = groupSecurityDao.getGroupSecuritiesByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupSecuritiesFromDB.size(), 0);
	}
	
	@Test
	@Rollback
	@Transactional
	public void getSecurityNamesByGroupIdTest(){
		groupDao.saveGroup(groupCommon);
		securityDao.saveSecurity(securityCommon);
		securityDao.saveSecurity(securityCommon2);
		for(GroupSecurity groupSecurity : groupSecurities){
			groupSecurity.setGroupId(groupCommon.getGroupId());
		}
		groupSecurities.get(0).setSecurityId(securityCommon.getSecurityId());
		groupSecurities.get(1).setSecurityId(securityCommon2.getSecurityId());
		groupSecurityDao.saveGroupSecurities(groupSecurities);
		List<String> groupSecuritiesFromDB = groupSecurityDao.getSecurityNamesByGroupId(groupCommon.getGroupId());
		Assert.assertEquals("[security name, security name]", groupSecuritiesFromDB.toString());
	}
}
