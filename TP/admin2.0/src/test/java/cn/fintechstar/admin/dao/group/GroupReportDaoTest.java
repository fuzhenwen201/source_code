package cn.fintechstar.admin.dao.group;

import java.util.Date;

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
import cn.fintechstar.admin.model.group.GroupReport;

@SpringBootTest
@RunWith(SpringJUnit4ClassRunner.class)
public class GroupReportDaoTest {

	@Autowired
	private IGroupDao groupDao;
	
	@Autowired
	private IGroupReportDao groupReportDao;
	
	private static GroupCommon groupCommon;
	
	private static GroupReport groupReport;
	
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
		
		groupReport = new GroupReport();
		groupReport.setIsReport(true);
		groupReport.setSmtpServer("smtp server");
		groupReport.setTemplatePath("template path");
		groupReport.setSmtpLogin("smtp login");
		groupReport.setSupportEmail("support email");
		groupReport.setIsCopyReport(true);
		groupReport.setSignature("signature");
	}
	
	@Test
	@Rollback
	@Transactional
	public void saveAndUpdateGroupReportTest(){
		//  --  save -- 
		groupDao.saveGroup(groupCommon);
		groupReport.setGroupId(groupCommon.getGroupId());
		groupReportDao.saveGroupReport(groupReport);
		GroupReport groupReportFromDB = groupReportDao.getGroupReportByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupReport.toString(), groupReportFromDB.toString());
		// -- update --
		groupReport.setIsReport(false);
		groupReport.setSmtpServer("test server");
		groupReport.setTemplatePath("test template path");
		groupReport.setSmtpLogin("test smtp login");
		groupReport.setSupportEmail("tes tsupport email");
		groupReport.setIsCopyReport(false);
		groupReport.setSignature("test signature");
		groupReportDao.updateGroupReport(groupReport);
		groupReportFromDB = groupReportDao.getGroupReportByGroupId(groupCommon.getGroupId());
		Assert.assertEquals(groupReport.toString(), groupReportFromDB.toString());
	}
}
