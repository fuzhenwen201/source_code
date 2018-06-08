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

import cn.fintechstar.admin.model.group.GroupCommon;
import cn.fintechstar.admin.model.group.GroupMargin;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class GroupMarginDaoTest {

	@Autowired
	private IGroupDao groupDao;
	
	@Autowired
	private IGroupMarginDao groupMarginDao;
	
	private static GroupCommon groupCommon;
	
	private static GroupMargin groupMargin;
	
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
		
		groupMargin = new GroupMargin();
		groupMargin.setCallLevel(1);
		groupMargin.setStopOutLevel("stop out level");
		groupMargin.setLevelFormat("level format");
		groupMargin.setFreeMargin("free margin");
		groupMargin.setVirtualCredit(11.11);
		groupMargin.setIsHedgedMarginCalulated(true);
		groupMargin.setIsSkipHedged(true);
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateGroupMarginTest(){
		// -- save --
		groupDao.saveGroup(groupCommon);
		groupMargin.setGroupId(groupCommon.getGroupId());
		groupMarginDao.saveGroupMargin(groupMargin);
		GroupMargin groupMarginFromDB = groupMarginDao.getGroupMarginByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupMargin.toString(), groupMarginFromDB.toString());
		// -- update --
		groupMargin.setCallLevel(2);
		groupMargin.setStopOutLevel("test level");
		groupMargin.setLevelFormat("test format");
		groupMargin.setFreeMargin("test margin");
		groupMargin.setVirtualCredit(22.22);
		groupMargin.setIsHedgedMarginCalulated(false);
		groupMargin.setIsSkipHedged(false);
		groupMarginDao.updateGroupMargin(groupMargin);
		groupMarginFromDB = groupMarginDao.getGroupMarginByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupMargin.toString(), groupMarginFromDB.toString());
	}
}
