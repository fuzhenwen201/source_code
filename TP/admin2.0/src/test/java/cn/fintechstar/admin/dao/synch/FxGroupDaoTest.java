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

import cn.fintechstar.admin.model.synch.GroupOfSqlite;
import cn.fintechstar.admin.synch.dao.IFxGroupDao;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class FxGroupDaoTest {

	@Autowired
	private IFxGroupDao fxGroupDao;
	
	private static List<GroupOfSqlite> groupOfSqlites;
	
	@BeforeClass
	public static void init(){
		groupOfSqlites = new ArrayList<>();
		
		GroupOfSqlite groupOfSqlite = new GroupOfSqlite();
		groupOfSqlite.setGroupId(1);
		groupOfSqlite.setGroupName("group1");
		groupOfSqlite.setSecurityId(1);
		groupOfSqlite.setTimePeriod("5;10;15;20");
		groupOfSqlite.setMaximumAmounts(1000);
		groupOfSqlite.setMinimumAmounts(500);
		groupOfSqlite.setGroupPassword("default");
		groupOfSqlite.setProfit(11.11);
		groupOfSqlite.setModifyTime("2017-11-13");
		groupOfSqlite.setDescription("test");
		
		GroupOfSqlite groupOfSqlite2 = new GroupOfSqlite();
		groupOfSqlite2.setGroupId(2);
		groupOfSqlite2.setGroupName("group2");
		groupOfSqlite2.setSecurityId(2);
		groupOfSqlite2.setGroupPassword("default");
		groupOfSqlite2.setTimePeriod("5;10;15;20");
		groupOfSqlite2.setMaximumAmounts(1000);
		groupOfSqlite2.setMinimumAmounts(500);
		groupOfSqlite2.setGroupPassword("default");
		groupOfSqlite2.setProfit(22.22);
		groupOfSqlite2.setModifyTime("2017-11-13");
		groupOfSqlite2.setDescription("test");
		
		groupOfSqlites.add(groupOfSqlite);
		groupOfSqlites.add(groupOfSqlite2);
	}
	
	@Test
	@Rollback
	@Transactional
	public void fxGroupDaoTest(){
		fxGroupDao.clearTable();
		fxGroupDao.saveFxGroups(groupOfSqlites);
	}
}
