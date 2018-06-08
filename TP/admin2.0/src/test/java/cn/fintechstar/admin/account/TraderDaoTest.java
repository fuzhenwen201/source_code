package cn.fintechstar.admin.account;

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

import cn.fintechstar.admin.dao.account.ITraderDao;
import cn.fintechstar.admin.dao.group.IGroupDao;
import cn.fintechstar.admin.model.account.Trader;
import cn.fintechstar.admin.model.account.TraderVo;
import cn.fintechstar.admin.model.group.GroupCommon;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class TraderDaoTest {

	@Autowired
	private ITraderDao traderDao;
	
	@Autowired
	private IGroupDao groupDao;
	
	private static Trader trader;
	
	private static GroupCommon groupCommon;
	
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
		
		trader = new Trader();
		trader.setPassword("password");
		trader.setFirstName("cherry");
		trader.setLastName("yu");
		trader.setEmail("123@qq.com");
		trader.setAddress("Nanshan");
		trader.setCity("SZ");
		trader.setState("Guangdong");
		trader.setCountry("CN");
		trader.setPin("86");
		trader.setIsDeleted(false);
		trader.setIsLogin(true);
		trader.setCreatedDate(new Date());
		trader.setUpdatedDate(new Date());
		trader.setDirtyBit(0);
		trader.setIsEnable(true);
		trader.setMetaQuotesId(4);
		
		trader.setLastLoginTime(new Date());
		trader.setTraderComment("test");
		trader.setPhone("12345678910");
		trader.setPhonePassword("phone password");
		trader.setIsAllowChangePassword(true);
		trader.setIsReadOnly(true);
		trader.setIsEnable(true);
		trader.setInvestorPassword("investor password");
		trader.setIdNumber(1111222233);
		trader.setLeverage(111);
		trader.setTaxRate(11.11);
		trader.setAgentAccount(55);
		trader.setIsSendReport(true);
		trader.setLeadSource("test");
		trader.setStatus("normal");
	}
	
	@Test
	@Rollback
	@Transactional
	public void traderDaoTest(){
		groupDao.saveGroup(groupCommon);
		trader.setGroupId(groupCommon.getGroupId());
		traderDao.saveTrader(trader);
		Trader traderFromDB = traderDao.getTraderById(trader.getClientId());
		Assert.assertEquals(trader.toString(), traderFromDB.toString());
		
		trader.setPassword("test");
		trader.setFirstName("test");
		trader.setLastName("test");
		trader.setEmail("test@qq.com");
		trader.setAddress("test");
		trader.setCity("test");
		trader.setState("test");
		trader.setCountry("test");
		trader.setPin("test");
		trader.setIsDeleted(false);
		trader.setDirtyBit(1);
		trader.setIsEnable(false);
		trader.setMetaQuotesId(1);

		trader.setTraderComment("test");
		trader.setPhone("9876543210");
		trader.setPhonePassword("test");
		trader.setIsAllowChangePassword(false);
		trader.setIsReadOnly(false);
		trader.setIsEnable(false);
		trader.setInvestorPassword("test");
		trader.setIdNumber(333222111);
		trader.setLeverage(22);
		trader.setTaxRate(22.22);
		trader.setAgentAccount(66);
		trader.setIsSendReport(false);
		trader.setLeadSource("test");
		trader.setStatus("test");
		
		traderDao.updateTrader(trader);
		traderFromDB = traderDao.getTraderById(trader.getClientId());
		Assert.assertEquals(trader.toString(), traderFromDB.toString());
		
		List<TraderVo> traderVos = traderDao.getTraders();
		Assert.assertTrue(traderVos.size() >= 1);
	}
}
