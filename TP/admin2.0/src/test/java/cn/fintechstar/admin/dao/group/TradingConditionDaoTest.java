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
import cn.fintechstar.admin.model.group.TradCondition;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class TradingConditionDaoTest {

	@Autowired
	private IGroupDao groupDao;
	
	@Autowired
	private ITradingConditionDao tradingConditionDao;
	
	private static GroupCommon groupCommon;
	
	private static TradCondition tradingCondition;
	
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
		
		tradingCondition = new TradCondition();
		tradingCondition.setMinimumBoAmount(11111L);
		tradingCondition.setMaximumBoAmount(22222L);
		tradingCondition.setPayout(80);
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateTradingConditionTest(){
		// -- save --
		groupDao.saveGroup(groupCommon);
		tradingCondition.setGroupId(groupCommon.getGroupId());
		tradingConditionDao.saveTradingCondition(tradingCondition);
		TradCondition tradingConditionFromDB = tradingConditionDao.getTradingConditionByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(tradingConditionFromDB.toString(), tradingCondition.toString());
		// -- update --
		tradingCondition.setMinimumBoAmount(22222L);
		tradingCondition.setMaximumBoAmount(33333L);
		tradingCondition.setPayout(60);
		tradingConditionDao.updateTradingCondition(tradingCondition);
		tradingConditionFromDB = tradingConditionDao.getTradingConditionByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(tradingConditionFromDB.toString(), tradingCondition.toString());
	}
	
}
