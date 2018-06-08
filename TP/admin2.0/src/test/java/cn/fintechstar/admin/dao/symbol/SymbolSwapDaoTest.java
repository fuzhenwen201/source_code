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
import cn.fintechstar.admin.dao.symbol.ISymbolSwapDao;
import cn.fintechstar.admin.model.security.Security;
import cn.fintechstar.admin.model.symbol.SymbolCommon;
import cn.fintechstar.admin.model.symbol.SymbolSwap;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class SymbolSwapDaoTest {
	
	@Autowired
	private ISymbolSwapDao symbolSwapDao;

	@Autowired
	private ISymbolDao symbolDao;
	
	@Autowired
	private ISecurityDao securityDao;
	
	private static Security security;
	
	private static SymbolCommon symbol;
	
	private static SymbolSwap symbolSwap;
	
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
		
		symbolSwap = new SymbolSwap();
		symbolSwap.setEnableSwap(true);
		symbolSwap.setType("type");
		symbolSwap.setThirdDaySwap("thirdDaySwap");
		symbolSwap.setLongPosition(11.11);
		symbolSwap.setShortPosition(12.22);
		symbolSwap.setUseOpenPrice(true);
		symbolSwap.setChangeVariation(true);
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateSymbolSwapTest(){
		// -- save --
		securityDao.saveSecurity(security);
		symbol.setSecurityId(security.getSecurityId());
		symbolDao.saveSymbol(symbol);
		symbolSwap.setSymbolId(symbol.getSymbolId());
		symbolSwapDao.saveSymbolSwap(symbolSwap);
		SymbolSwap symbolSwapFromDB = symbolSwapDao.getSymbolSwapBySymbolId(symbol.getSymbolId());
		Assert.assertEquals(symbolSwap.toString(), symbolSwapFromDB.toString());
		// -- update --
		symbolSwap.setEnableSwap(false);
		symbolSwap.setType("test");
		symbolSwap.setLongPosition(33.33);
		symbolSwap.setShortPosition(44.44);
		symbolSwap.setThirdDaySwap("test");
		symbolSwap.setUseOpenPrice(false);
		symbolSwap.setChangeVariation(false);
		symbolSwapDao.updateSymbolSwap(symbolSwap);
		symbolSwapFromDB = symbolSwapDao.getSymbolSwapBySymbolId(symbol.getSymbolId());
		Assert.assertEquals(symbolSwap.toString(), symbolSwapFromDB.toString());	
	}
}
