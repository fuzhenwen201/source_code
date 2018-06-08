package com.broctagon.trad.networkconfiguration.db.test;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.trad.networkconfiguration.db.SpringBootApp;
import com.broctagon.trad.networkconfiguration.db.daotest.BoCalculationDaoTest;
import com.broctagon.trad.networkconfiguration.db.daotest.GroupBoSymbolsDaoTest;
import com.broctagon.trad.networkconfiguration.db.daotest.GroupSpreadDaoTest;
import com.broctagon.trad.networkconfiguration.db.daotest.SecuritySpreadDaoTest;
import com.broctagon.trad.networkconfiguration.db.daotest.SgIpUrlMapDaoTest;
import com.broctagon.trad.networkconfiguration.db.daotest.SymbolSpreadDaoTest;

@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes=SpringBootApp.class)
public class SpringBootAppTest {

	@Autowired private BoCalculationDaoTest boCalculationDaoTest;
	@Autowired private GroupBoSymbolsDaoTest groupBoSymbolsDaoTest;
	@Autowired private GroupSpreadDaoTest groupSpreadDaoTest;
	@Autowired private SecuritySpreadDaoTest securitySpreadDaoTest;
	@Autowired private SgIpUrlMapDaoTest sgIpUrlMapDaoTest;
	@Autowired private SymbolSpreadDaoTest symbolSpreadDaoTest;
	
	@Test
	public void testAllDao(){
		System.out.println("start testAllDao.");
		
	//	boCalculationDaoTest.testAll();
	//	groupBoSymbolsDaoTest.testAll();
	//	groupSpreadDaoTest.testAll();
	//	securitySpreadDaoTest.testAll();
		
/*		sgIpUrlMapDaoTest.testAll();
		symbolSpreadDaoTest.testAll();*/
		
		System.out.println("end testAllDao.");
	}
	
}
