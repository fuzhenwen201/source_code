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
import cn.fintechstar.admin.dao.symbol.ISymbolCalculationDao;
import cn.fintechstar.admin.dao.symbol.ISymbolDao;
import cn.fintechstar.admin.model.security.Security;
import cn.fintechstar.admin.model.symbol.SymbolCalculation;
import cn.fintechstar.admin.model.symbol.SymbolCommon;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class SymbolCalculationDaoTest {

	@Autowired
	private ISymbolCalculationDao symbolCalculationDao;
	
	@Autowired
	private ISymbolDao symbolDao;
	
	@Autowired
	private ISecurityDao securityDao;
	
	private static Security security;
	
	private static SymbolCommon symbol;
	
	private static SymbolCalculation symbolCalculation;
	
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
		
		symbolCalculation = new SymbolCalculation();
		symbolCalculation.setContractSize(0.05f);
		symbolCalculation.setInitialMargin(11.5);
		symbolCalculation.setMaintenace(0.05f);
		symbolCalculation.setHedged(0.8f);
		symbolCalculation.setTickSize(0.99f);
		symbolCalculation.setMarginPercentage(0.89);
		symbolCalculation.setMarginCal("marginCal");
		symbolCalculation.setProfitCal("profitCal");
		symbolCalculation.setShmMode(true);
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateSymbolCalculationTest(){
		// -- save --
		securityDao.saveSecurity(security);
		symbol.setSecurityId(security.getSecurityId());
		symbolDao.saveSymbol(symbol);
		symbolCalculation.setSymbolId(symbol.getSymbolId());
		symbolCalculationDao.saveSymbolCalculation(symbolCalculation);
		SymbolCalculation symbolCalculationFromDB = symbolCalculationDao.getSymbolCalculationBySymbolId(symbol.getSymbolId());
		Assert.assertEquals(symbolCalculationFromDB.toString(), symbolCalculation.toString());
		// -- update --
		symbolCalculation.setContractSize(0.89f);
		symbolCalculation.setInitialMargin(11.11);
		symbolCalculation.setHedged(0.5f);
		symbolCalculation.setProfitCal("test");
		symbolCalculationDao.updateSymbolCalculation(symbolCalculation);
		symbolCalculationFromDB = symbolCalculationDao.getSymbolCalculationBySymbolId(symbol.getSymbolId());
		Assert.assertEquals(symbolCalculationFromDB.toString(), symbolCalculation.toString());
	}

}
