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
import cn.fintechstar.admin.dao.symbol.IBoSymbolDao;
import cn.fintechstar.admin.dao.symbol.ISymbolDao;
import cn.fintechstar.admin.model.security.Security;
import cn.fintechstar.admin.model.symbol.BoSymbol;
import cn.fintechstar.admin.model.symbol.SymbolCommon;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class BoSymbolDaoTest {

	@Autowired
	private IBoSymbolDao boSymbolDao;
	
	@Autowired
	private ISymbolDao symbolDao;
	
	@Autowired
	private ISecurityDao securityDao;
	
	private static Security security;
	
	private static SymbolCommon symbol;
	
	private static BoSymbol boSymbol;
	
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
		
		boSymbol = new BoSymbol();
		boSymbol.setBoCalType(1);
		boSymbol.setReturnRate(11.1);
	}

	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateBoSymbolTest(){
		// -- save --
		securityDao.saveSecurity(security);
		symbol.setSecurityId(security.getSecurityId());
		symbolDao.saveSymbol(symbol);
		boSymbol.setSymbolId(symbol.getSymbolId());
		boSymbolDao.saveBoSymbol(boSymbol);
		BoSymbol boSymbolFromDB = boSymbolDao.getBoSymbolBySymbolId(symbol.getSymbolId());
		Assert.assertEquals(boSymbol.toString(), boSymbolFromDB.toString());		
		// -- update --
		boSymbol.setBoCalType(0);
		boSymbolFromDB.setReturnRate(12.2);
		boSymbolDao.updateBoSymbol(boSymbol);
		boSymbolFromDB = boSymbolDao.getBoSymbolBySymbolId(symbol.getSymbolId());
		Assert.assertEquals(boSymbol.toString(), boSymbolFromDB.toString());	
	}
}
