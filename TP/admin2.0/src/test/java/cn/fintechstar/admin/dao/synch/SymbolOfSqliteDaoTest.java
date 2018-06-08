package cn.fintechstar.admin.dao.synch;

import java.util.ArrayList;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.annotation.Rollback;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.transaction.annotation.Transactional;

import cn.fintechstar.admin.model.synch.SymbolOfSqlite;
import cn.fintechstar.admin.synch.dao.ISymbolOfSqliteDao;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class SymbolOfSqliteDaoTest {

	
	@Autowired
	private ISymbolOfSqliteDao symbolOfSqliteDao;
	
	private static List<SymbolOfSqlite> symbolOfSqlites;
	
	@BeforeClass
	public static void init(){
		symbolOfSqlites = new ArrayList<>();
		
		SymbolOfSqlite symbolOfSqlite = new SymbolOfSqlite();
		symbolOfSqlite.setSymbol("AUDCAD");
		symbolOfSqlite.setSymbolSpread(5);
		symbolOfSqlite.setOperatorId(4);
		symbolOfSqlite.setModifyTime("2017-11-13");
		symbolOfSqlite.setDescribe("test");
		
		SymbolOfSqlite symbolOfSqlite2 = new SymbolOfSqlite();
		symbolOfSqlite2.setSymbol("CADUSD");
		symbolOfSqlite2.setSymbolSpread(15);
		symbolOfSqlite2.setOperatorId(4);
		symbolOfSqlite2.setModifyTime("2017-11-13");
		symbolOfSqlite2.setDescribe("test");
		
		symbolOfSqlites.add(symbolOfSqlite);
		symbolOfSqlites.add(symbolOfSqlite2);
	}
	
	@Test
	@Rollback
	@Transactional
	public void symbolOfSqliteDaoTest(){
		symbolOfSqliteDao.clearTable();
		symbolOfSqliteDao.saveSymbols(symbolOfSqlites);
	}
}
