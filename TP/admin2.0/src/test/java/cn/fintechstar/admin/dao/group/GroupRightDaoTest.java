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
import cn.fintechstar.admin.model.group.GroupRight;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class GroupRightDaoTest {

	@Autowired
	private IGroupDao groupDao;
	
	@Autowired
	private IGroupRightDao groupRightDao;
	
	private static GroupCommon groupCommon;
	
	private static GroupRight groupRight;
	
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
		
		groupRight = new GroupRight();
		groupRight.setTimeout(500);
		groupRight.setNewsView("news view");
		groupRight.setNewsLanguage("news language");
		groupRight.setMaxSymbol(5);
		groupRight.setMaxOrder(5);
		groupRight.setTradeSignals("trade signals");
		groupRight.setIsMailSystem(1);
		groupRight.setIsSwap(1);
		groupRight.setIsEA(1);
		groupRight.setIsPendingOrderExpiry(1);
		groupRight.setIsTrailingStop(1);
		groupRight.setIsReqPriceInIE(1);
		groupRight.setIsHedgeProhibit(1);
		groupRight.setIsFIFO(1);
		groupRight.setIsParClose(1);
		groupRight.setIsReqPriceInIE(1);
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateGroupRightTest(){
		// -- save --
		groupDao.saveGroup(groupCommon);
		groupRight.setGroupId(groupCommon.getGroupId());
		groupRightDao.saveGroupRight(groupRight);
		GroupRight groupRightFromDB = groupRightDao.getGroupRightByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupRight.toString(), groupRightFromDB.toString());
		// -- update --
		groupRight.setTimeout(100);
		groupRight.setNewsView("test news view");
		groupRight.setNewsLanguage("test news language");
		groupRight.setMaxSymbol(10);
		groupRight.setMaxOrder(10);
		groupRight.setTradeSignals("test trade signals");
		groupRight.setIsMailSystem(0);
		groupRight.setIsSwap(0);
		groupRight.setIsEA(0);
		groupRight.setIsPendingOrderExpiry(0);
		groupRight.setIsTrailingStop(0);
		groupRight.setIsReqPriceInIE(0);
		groupRight.setIsHedgeProhibit(0);
		groupRight.setIsFIFO(0);
		groupRight.setIsParClose(0);
		groupRight.setIsReqPriceInIE(0);
		groupRightDao.updateGroupRight(groupRight);
		groupRightFromDB = groupRightDao.getGroupRightByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupRight.toString(), groupRightFromDB.toString());
	}
}
