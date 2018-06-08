package cn.fintechstar.admin.dao.group;

import java.util.Date;
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

import cn.fintechstar.admin.model.group.GroupCommon;
import cn.fintechstar.admin.model.group.GroupDetailsVo;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class GroupDaoTest {
	
	@Autowired
	private IGroupDao groupDao;
	
	private static GroupCommon groupCommon;
	
	@BeforeClass
	public static void init(){
		groupCommon = new GroupCommon();
		groupCommon.setGroupName("unit test");
		groupCommon.setIsEnable(1);
		groupCommon.setOtp("TOPT SHA-256");
		groupCommon.setIsEnable(1);
		groupCommon.setOwnerName("Broctagon Fintech Group");
		groupCommon.setSupportedPage("supported page");
		groupCommon.setDefaultDeposit(11.11);
		groupCommon.setDepositCurrency("USD");
		groupCommon.setDefaultLeverage(11.11);
		groupCommon.setAnnualInterest(11.11);
		groupCommon.setIsDeleted(0);
		groupCommon.setCreatedTime(new Date());
		groupCommon.setModifiedTime(new Date());
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateGroupTest(){
		// -- save --
		groupDao.saveGroup(groupCommon);
		GroupCommon groupCommonFromDB = groupDao.getGroupCommonByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupCommon.toString(), groupCommonFromDB.toString());
		// -- update --
		groupCommon.setGroupName("update unit test");
		groupCommon.setSupportedPage("update test");
		groupDao.updateGroup(groupCommon);
		groupCommonFromDB = groupDao.getGroupCommonByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupCommon.toString(), groupCommonFromDB.toString());
	}

	@Test
	@Rollback
	@Transactional
	public void getGroupsTest(){
		groupDao.saveGroup(groupCommon);
		List<GroupDetailsVo> groupDetailsVos = groupDao.getGroups();
		Assert.assertNotEquals(groupDetailsVos.size(), 0);
	}
}
