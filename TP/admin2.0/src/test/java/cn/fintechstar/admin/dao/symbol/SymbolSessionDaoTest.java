package cn.fintechstar.admin.dao.symbol;

import java.util.ArrayList;
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
import cn.fintechstar.admin.dao.symbol.ISymbolSessionDao;
import cn.fintechstar.admin.model.security.Security;
import cn.fintechstar.admin.model.symbol.SymbolCommon;
import cn.fintechstar.admin.model.symbol.SymbolSession;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class SymbolSessionDaoTest {
	
	@Autowired
	private ISymbolSessionDao symbolSessionDao;
	
	@Autowired
	private ISymbolDao symbolDao;
	
	@Autowired
	private ISecurityDao securityDao;
	
	private static Security security;
	
	private static SymbolCommon symbol;
	
	private static List<SymbolSession> symbolSessions;
	
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
		
		symbolSessions = new ArrayList<>();
		SymbolSession symbolSession = new SymbolSession();
		symbolSession.setDay("Monday");
		SymbolSession symbolSession2 = new SymbolSession();
		symbolSession.setDay("Tuesday");
		SymbolSession symbolSession3 = new SymbolSession();
		symbolSession.setDay("Wednesday");
		symbolSessions.add(symbolSession);
		symbolSessions.add(symbolSession2);
		symbolSessions.add(symbolSession3);
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateSymbolSessionTest(){
		// -- save --
		securityDao.saveSecurity(security);
		symbol.setSecurityId(security.getSecurityId());
		symbolDao.saveSymbol(symbol);
		for (SymbolSession symbolSession : symbolSessions) {
			symbolSession.setSymbolId(symbol.getSymbolId());
		}
		symbolSessionDao.saveSymbolSessions(symbolSessions);
		List<SymbolSession> symbolSessionsFromDB = symbolSessionDao.getSymbolSessionsBySymbolId(symbol.getSymbolId());
		Assert.assertEquals(symbolSessions.toString(), symbolSessionsFromDB.toString());
		System.err.println(symbolSessions);
		System.err.println(symbolSessionsFromDB);
		// -- update --
		for (SymbolSession symbolSession : symbolSessions) {
			symbolSession.setDay("Friday");
		}
		symbolSessionDao.updateSymbolSessions(symbolSessions);
		symbolSessionsFromDB = symbolSessionDao.getSymbolSessionsBySymbolId(symbol.getSymbolId());
		Assert.assertEquals(symbolSessions.toString(), symbolSessionsFromDB.toString());
	}
	
}
