package cn.fintechstar.admin.dao.symbol;

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
import cn.fintechstar.admin.model.security.Security;
import cn.fintechstar.admin.model.symbol.SymbolCommon;
import cn.fintechstar.admin.model.symbol.SymbolVo;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class SymbolDaoTest {

	@Autowired
	private ISymbolDao symbolDao;
	
	@Autowired
	private ISecurityDao securityDao;
	
	private static Security security;
	
	private static SymbolCommon symbol;
	
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
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveSymbolTest(){
		securityDao.saveSecurity(security);
		symbol.setSecurityId(security.getSecurityId());
		symbolDao.saveSymbol(symbol);
		Assert.assertNotEquals(symbol.getSymbolId(), 0);
	}
	
	@Test
	@Rollback
	@Transactional
	public void getSymbolNamesBySecurityIdTest(){
		securityDao.saveSecurity(security);
		symbol.setSecurityId(security.getSecurityId());
		symbol.setSymbol("symbol1");
		symbolDao.saveSymbol(symbol);
		symbol.setSymbol("symbol2");
		symbolDao.saveSymbol(symbol);
		List<String> symbolNames = symbolDao.getSymbolNamesBySecurityId(security.getSecurityId());
		Assert.assertEquals(symbolNames.toString(), "[symbol1, symbol2]");
	}
	
	@Test
	@Rollback
	@Transactional
	public void updateSymbolTest(){
		securityDao.saveSecurity(security);
		symbol.setSecurityId(security.getSecurityId());
		symbolDao.saveSymbol(symbol);
		symbol.setSymbol("update test");
		symbolDao.updateSymbol(symbol);
		SymbolCommon symbolFromDatabase = symbolDao.getSymbolBySymbolId(symbol.getSymbolId());
		Assert.assertEquals(symbolFromDatabase.getSymbol(), "update test");
		List<SymbolVo> symbols = symbolDao.getSymbols();
		Assert.assertNotEquals(symbols.size(), 0);
	}

}
