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
import cn.fintechstar.admin.dao.symbol.ISymbolDao;
import cn.fintechstar.admin.model.group.GroupCommon;
import cn.fintechstar.admin.model.group.GroupSymbol;
import cn.fintechstar.admin.model.security.Security;
import cn.fintechstar.admin.model.symbol.SymbolCommon;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class GroupSymbolDaoTest {

	@Autowired
	private IGroupDao groupDao;
	
	@Autowired
	private ISecurityDao securityDao;
	
	@Autowired
	private ISymbolDao symbolDao;
	
	@Autowired
	private IGroupSymbolDao groupSymbolDao;
	
	private static GroupCommon groupCommon;
	
	private static Security securityCommon;
	
	private static Security securityCommon2;
	
	private static SymbolCommon symbol;
	
	private static SymbolCommon symbol2;
	
	private static List<GroupSymbol> groupSymbols;
	
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
		securityCommon2.setType("bo");
		securityCommon2.setCreatedTime(new Date());
		securityCommon2.setModifiedTime(new Date());
		
		symbol = new SymbolCommon();
		symbol.setSymbol("AUDCAD_UNITTEST");
		symbol.setSource("unit test");
		symbol.setDigit(5);
		symbol.setDescription("unit test");
		symbol.setExecutionMode("execution mode");
		symbol.setCurrency("currency");
		symbol.setTrade("trade");
		symbol.setBackgroundColor("black");
		symbol.setMarginCurrency("marign currency");
		symbol.setMaxLot(100);
		symbol.setDefaultSpread("10");
		symbol.setLong(true);
		symbol.setLimitStopLevel(5);
		symbol.setFreezeLevel(5);
		symbol.setSpreadBalAsk(10);
		symbol.setSpreadBalBid(9);
		symbol.setOrders("orders");
		symbol.setSymbolSourceId(131);
		
		symbol2 = new SymbolCommon();
		symbol2.setSymbol("AUDCAD_UNITTEST");
		symbol2.setSource("unit test");
		symbol2.setDigit(5);
		symbol2.setDescription("unit test");
		symbol2.setExecutionMode("execution mode");
		symbol2.setCurrency("currency");
		symbol2.setTrade("trade");
		symbol2.setBackgroundColor("black");
		symbol2.setMarginCurrency("marign currency");
		symbol2.setMaxLot(100);
		symbol2.setDefaultSpread("10");
		symbol2.setLong(true);
		symbol2.setLimitStopLevel(5);
		symbol2.setFreezeLevel(5);
		symbol2.setSpreadBalAsk(10);
		symbol2.setSpreadBalBid(9);
		symbol2.setOrders("orders");
		symbol2.setSymbolSourceId(131);
		
		groupSymbols = new ArrayList<>();
		
		GroupSymbol groupSymbol = new GroupSymbol();
		groupSymbol.setLongPositionSwap(11.11);
		groupSymbol.setShortPositionSwap(11.11);
		groupSymbol.setMarginPercentage(11.11);
		
		GroupSymbol groupSymbol2 = new GroupSymbol();
		groupSymbol2.setLongPositionSwap(11.11);
		groupSymbol2.setShortPositionSwap(11.11);
		groupSymbol2.setMarginPercentage(11.11);
		
		groupSymbols.add(groupSymbol);
		groupSymbols.add(groupSymbol2);
		
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateGroupSymbolTest(){
		// -- save --
		groupDao.saveGroup(groupCommon);
		symbolDao.saveSymbol(symbol);
		symbolDao.saveSymbol(symbol2);
		securityDao.saveSecurity(securityCommon);
		securityDao.saveSecurity(securityCommon2);
		for(GroupSymbol groupSymbol : groupSymbols){
			groupSymbol.setGroupId(groupCommon.getGroupId());
		}
		groupSymbols.get(0).setSymbolId(symbol.getSymbolId());
		groupSymbols.get(1).setSymbolId(symbol2.getSymbolId());
		groupSymbolDao.saveGroupSymbols(groupSymbols);
		List<GroupSymbol> groupSymbolsFromDB = groupSymbolDao.getGroupSymbolsByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupSymbolsFromDB.toString(), groupSymbols.toString());
		// -- delete --
		groupSymbolDao.deleteGroupSymbolsByGroupId(groupCommon.getGroupId());
		groupSymbolsFromDB = groupSymbolDao.getGroupSymbolsByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupSymbolsFromDB.size(), 0);
	}
}
