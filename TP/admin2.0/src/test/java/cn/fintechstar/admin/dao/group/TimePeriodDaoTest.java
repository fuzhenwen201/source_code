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

import cn.fintechstar.admin.model.group.GroupCommon;
import cn.fintechstar.admin.model.group.TimePeriod;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class TimePeriodDaoTest {

	@Autowired
	private IGroupDao groupDao;
	
	@Autowired
	private ITimePeriodDao timePeriodDao;
	
	private static GroupCommon groupCommon;
	
	private static List<TimePeriod> timePeriods;
	
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
		
		timePeriods = new ArrayList<>();
		
		TimePeriod timePeriod = new TimePeriod();
		timePeriod.setPeriod(5);
		timePeriod.setProfitPercentage(80);
		
		TimePeriod timePeriod2 = new TimePeriod();
		timePeriod2.setPeriod(10);
		timePeriod2.setProfitPercentage(80);
		
		timePeriods.add(timePeriod);
		timePeriods.add(timePeriod2);
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateTimePeriodTest(){
		// -- save --
		groupDao.saveGroup(groupCommon);
		for(TimePeriod timePeriod : timePeriods){
			timePeriod.setGroupId(groupCommon.getGroupId());
		}
		timePeriodDao.saveTimePeriods(timePeriods);
		List<TimePeriod> timePeriodsFromDB = timePeriodDao.getTimePeriodsByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(timePeriodsFromDB.toString(), timePeriods.toString());
		// -- delete --
		timePeriodDao.deleteTimePeriodsByGroupId(groupCommon.getGroupId());
		timePeriodsFromDB = timePeriodDao.getTimePeriodsByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(timePeriodsFromDB.size(), 0);
	}
}
