package cn.fintechstar.admin.account;

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

import cn.fintechstar.admin.dao.account.IAccountDao;
import cn.fintechstar.admin.dao.account.ITraderDao;
import cn.fintechstar.admin.dao.group.IGroupDao;
import cn.fintechstar.admin.model.account.Account;
import cn.fintechstar.admin.model.account.Trader;
import cn.fintechstar.admin.model.group.GroupCommon;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class AccountDaoTest {

	@Autowired
	private IAccountDao accountDao;
	
	@Autowired
	private ITraderDao traderDao;
	
	@Autowired
	private IGroupDao groupDao;
	
	private static Trader trader;
	
	private static Account account;
	
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
		
		account = new Account();
		account.setBalance(5000000);
		account.setPrevBalance(1000000);
		account.setChangeDeposit(100000000);
		account.setMargin(500000);
		account.setFreeMargin(1000000);
		account.setCloseBalance(1500000);
		account.setDirtyBit(1);
		account.setBoAccount(1000000);
		account.setFxAccount(10000000);
	}
	
	@Test
	@Rollback
	@Transactional
	public void accountDaoTest(){
		// -- save --
		groupDao.saveGroup(groupCommon);
		trader.setGroupId(groupCommon.getGroupId());
		traderDao.saveTrader(trader);
		account.setGroupId(groupCommon.getGroupId());
		account.setClientId(trader.getClientId());
		accountDao.saveAccount(account);
		Assert.assertTrue(account.getAccountId() != 0);
	}
}
