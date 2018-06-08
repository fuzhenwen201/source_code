package cn.fintechstar.admin.dao.group;

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

import cn.fintechstar.admin.model.group.GroupArchieve;
import cn.fintechstar.admin.model.group.GroupCommon;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class GroupArchieveDaoTest {

	@Autowired
	private IGroupDao groupDao;
	
	@Autowired
	private IGroupArchieveDao groupArchieveDao;
	
	private static GroupCommon groupCommon;
	
	private static GroupArchieve groupArchieve;
	
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
		
		groupArchieve = new GroupArchieve();
		groupArchieve.setInActivePeriod(5);
		groupArchieve.setMaxBalance(11.11);
		groupArchieve.setPendingDeletedOrder(11);
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateGroupArchieveTest(){
		// -- save --
		groupDao.saveGroup(groupCommon);
		groupArchieve.setGroupId(groupCommon.getGroupId());
		groupArchieveDao.saveGroupArchieve(groupArchieve);
		GroupArchieve groupArchieveFromDB = groupArchieveDao.getGroupArchieveByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupArchieveFromDB.toString(), groupArchieve.toString());
		// -- update --
		groupArchieve.setInActivePeriod(2);
		groupArchieve.setMaxBalance(22.22);
		groupArchieve.setPendingDeletedOrder(22);
		groupArchieveDao.updateGroupArchieve(groupArchieve);
		groupArchieveFromDB = groupArchieveDao.getGroupArchieveByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupArchieveFromDB.toString(), groupArchieve.toString());
	}
}
