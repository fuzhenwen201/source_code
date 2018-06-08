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

import cn.fintechstar.admin.model.synch.SecurityOfSqlite;
import cn.fintechstar.admin.synch.dao.IFxSecurityDao;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class FxSecurityDaoTest {
	
	@Autowired
	private IFxSecurityDao fxSecurityDao;
	
	private static List<SecurityOfSqlite> securityOfSqlites;
	
	@BeforeClass
	public static void init(){
		securityOfSqlites = new ArrayList<>();

		SecurityOfSqlite securityOfSqlite = new SecurityOfSqlite();
		securityOfSqlite.setSecurityId(1);
		securityOfSqlite.setSecurityName("securityName1");
		securityOfSqlite.setSymbolList("AUDCAD;CADJPY;SGDJPY");
		securityOfSqlite.setAdminId(4);
		securityOfSqlite.setModifyTime("2017-11-13");
		securityOfSqlite.setDescription("test");
		
		SecurityOfSqlite securityOfSqlite2 = new SecurityOfSqlite();
		securityOfSqlite2.setSecurityId(2);
		securityOfSqlite2.setSecurityName("securityName2");
		securityOfSqlite2.setSymbolList("SGDUSD;CADUSD;");
		securityOfSqlite2.setAdminId(4);
		securityOfSqlite2.setModifyTime("2017-11-13");
		securityOfSqlite2.setDescription("test");
		
		securityOfSqlites.add(securityOfSqlite);
		securityOfSqlites.add(securityOfSqlite2);
	}
	
	@Test
	@Rollback
	@Transactional
	public void fxSecurityDaoTest(){
		fxSecurityDao.clearTable();
		fxSecurityDao.saveFxSecurities(securityOfSqlites);
	}
}
