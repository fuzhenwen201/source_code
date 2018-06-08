package cn.fintechstar.admin.dao.symbol;

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
import cn.fintechstar.admin.dao.symbol.ISymbolFilterationDao;
import cn.fintechstar.admin.model.security.Security;
import cn.fintechstar.admin.model.symbol.SymbolCommon;
import cn.fintechstar.admin.model.symbol.SymbolFilteration;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class SymbolFilterationDaoTest {

	@Autowired
	private ISymbolFilterationDao symbolFilterationDao;
	
	@Autowired
	private ISymbolDao symbolDao;
	
	@Autowired
	private ISecurityDao securityDao;
	
	private static Security security;
	
	private static SymbolCommon symbol;
	
	private static SymbolFilteration symbolFilteration;
	
	@BeforeClass
	public static void init(){
		security = new Security();
		security.setType("bo");
		security.setSecurityName("unit test");
		security.setDescription("junit test from SymbolDaoTest");
		
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
		
		symbolFilteration = new SymbolFilteration();
		symbolFilteration.setRealtimeDataFeed(true);
		symbolFilteration.setSaveAllIncomingPrice(true);
		symbolFilteration.setFiltrationLevel(0);
		symbolFilteration.setAutomaticLimit(11.1);
		symbolFilteration.setFilter(5);
		symbolFilteration.setIgnoreQuot(10);
		symbolFilteration.setSmoothing(15);
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateSymbolFilterationTest(){
		// -- save --
		securityDao.saveSecurity(security);
		symbol.setSecurityId(security.getSecurityId());
		symbolDao.saveSymbol(symbol);
		symbolFilteration.setSymbolId(symbol.getSymbolId());
		symbolFilterationDao.saveSymbolFilteration(symbolFilteration);
		SymbolFilteration symbolFilterationFromDB = symbolFilterationDao.getSymbolFilterationBySymbolId(symbol.getSymbolId());
		Assert.assertEquals(symbolFilteration.toString(), symbolFilterationFromDB.toString());
		// -- update --
		symbolFilteration.setRealtimeDataFeed(false);
		symbolFilteration.setSaveAllIncomingPrice(false);
		symbolFilteration.setFiltrationLevel(1);
		symbolFilteration.setAutomaticLimit(22.2);
		symbolFilteration.setFilter(6);
		symbolFilteration.setIgnoreQuot(12);
		symbolFilteration.setSmoothing(18);
		symbolFilterationDao.updateSymbolFilteration(symbolFilteration);
		symbolFilterationFromDB = symbolFilterationDao.getSymbolFilterationBySymbolId(symbol.getSymbolId());
		Assert.assertEquals(symbolFilteration.toString(), symbolFilterationFromDB.toString());
	}
	
}
