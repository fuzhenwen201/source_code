package com.broctagon.trad.reportserver.dao;

import java.math.BigDecimal;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import javax.sql.rowset.serial.SerialException;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.trad.reportserver.config.DruidConfig;
import com.broctagon.trad.reportserver.model.User;
import com.broctagon.trad.reportserver.util.DateUtil;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = {DruidConfig.class})
public class UserDaoTest {
	
	@Autowired
	private UserDao userDao;
	
//	@Test
	public void testGetUserByLoginId(){
		User user = userDao.getUserByLoginId(3278251);
		System.out.println(user.getEmail());
	}
	
//	@Test
	public void testGetAll(){
		List<Integer> ids = new ArrayList<>();
		ids.add(3278251);
		ids.add(3284832);
		ids.add(3296079);
		ids.add(3297404);
		List<User> users =  userDao.getUsersByLoginIds(ids);
		System.out.println(users);
	}
	
//	@Test
	public void addUser(){
		User user = new User();
		user.setLogin(11);
		user.setGroup("JTVBOC");
		user.setEnable(1);
		user.setEnable_change_pass(1);
		user.setEnable_readOnly(0);
		user.setEnableOpt(0);
		user.setPassword_phone("");
		user.setName("huangguiying");
		user.setCity("nanchangshi");
		user.setState("jiangxisheng");
		user.setZipCode("330000");
		user.setAddress("jiangxishengnanchangshigaoxinjishukaifaquchangdongzhenrixincudanjiazirancun67");
		user.setPhone("+86-13870939153");
		user.setEmail("2879035889@qq.com");
		user.setCountry("China");
		user.setComment("");
		user.setId("360121197102172489");
		user.setState("");
		user.setRegDate(DateUtil.getTimestamp(new Date()));
		user.setLastDate(DateUtil.getTimestamp(new Date()));
		user.setLeverage(100);
		user.setAgent_account(3284824);
		user.setTimestamp(1497681147);
		user.setBalance(0);
		user.setPrevMonthBalance(0.0);
		user.setPrevBalance(0.0);
		user.setCredit(0.0);
		user.setInterestrate(0.0);
		user.setTaxes(0.0);
		user.setSend_report(1);
		user.setMqid(new BigDecimal("476267619"));
		user.setUserColor(0);
		user.setEquity(0.0);
		user.setMargin(0.0);
		user.setLead_source("p");
		user.setMargin_free(0.0);
		user.setCurrency("USD");
		user.setStatus("test");
		user.setModify_time(DateUtil.getTimestamp(new Date()));
		userDao.addUser(user);
	}
	
	@Test
	public void addUsers() throws SerialException, SQLException{
		List<User> users = new ArrayList<>();
		byte[] bytes = new byte[]{1, 0, 0, 0, 1, 1, 0};
		User u1 = new User();
		u1.setLogin(145213678);
		u1.setGroup("SSSSS");
		u1.setEnable(1);
		u1.setEnable_change_pass(1);
		u1.setEnable_readOnly(0);
		u1.setEnableOpt(0);
		u1.setPassword_phone("");
		u1.setName("huangguiying");
		u1.setCity("nanchangshi");
		u1.setState("jiangxisheng");
		u1.setZipCode("330000");
		u1.setAddress("jiangxishengnanchangshigaoxinjishukaifaquchangdongzhenrixincudanjiazirancun67");
		u1.setPhone("+86-13870939153");
		u1.setEmail("2879035889@qq.com");
		u1.setCountry("China");
		u1.setComment("");
		u1.setId("360121197102172489");
		u1.setState("");
		u1.setRegDate(DateUtil.getTimestamp(new Date()));
		u1.setLastDate(DateUtil.getTimestamp(new Date()));
		u1.setLeverage(100);
		u1.setAgent_account(3284824);
		u1.setTimestamp(1497681147);
		u1.setBalance(0);
		u1.setPrevMonthBalance(0.0);
		u1.setPrevBalance(0.0);
		u1.setCredit(0.0);
		u1.setInterestrate(0.0);
		u1.setTaxes(0.0);
		u1.setSend_report(1);
		u1.setMqid(new BigDecimal("476267619"));
		u1.setUserColor(0);
		u1.setEquity(0.0);
		u1.setMargin(0.0);
		u1.setLead_source("p");
		u1.setMargin_free(0.0);
		u1.setCurrency("USD");
		u1.setStatus("test");
		u1.setApi_data(bytes);
		u1.setModify_time(DateUtil.getTimestamp(new Date()));
		User u2 = new User();
		u2.setLogin(7819415);
		u2.setGroup("WWWW");
		u2.setEnable(1);
		u2.setEnable_change_pass(1);
		u2.setEnable_readOnly(0);
		u2.setEnableOpt(0);
		u2.setPassword_phone("");
		u2.setName("huangguiying");
		u2.setCity("nanchangshi");
		u2.setState("jiangxisheng");
		u2.setZipCode("330000");
		u2.setAddress("jiangxishengnanchangshigaoxinjishukaifaquchangdongzhenrixincudanjiazirancun67");
		u2.setPhone("+86-13870939153");
		u2.setEmail("2879035889@qq.com");
		u2.setCountry("China");
		u2.setComment("");
		u2.setId("360121197102172489");
		u2.setState("");
		u2.setRegDate(DateUtil.getTimestamp(new Date()));
		u2.setLastDate(DateUtil.getTimestamp(new Date()));
		u2.setLeverage(100);
		u2.setAgent_account(3284824);
		u2.setTimestamp(1497681147);
		u2.setBalance(0);
		u2.setPrevMonthBalance(0.0);
		u2.setPrevBalance(0.0);
		u2.setCredit(0.0);
		u2.setInterestrate(0.0);
		u2.setTaxes(0.0);
		u2.setSend_report(1);
		u2.setMqid(new BigDecimal("476267619"));
		u2.setUserColor(0);
		u2.setEquity(0.0);
		u2.setMargin(0.0);
		u2.setLead_source("p");
		u2.setMargin_free(0.0);
		u2.setCurrency("USD");
		u2.setStatus("test");
		u2.setApi_data(bytes);
		u2.setModify_time(DateUtil.getTimestamp(new Date()));
		users.add(u1);
		users.add(u2);
		userDao.addUsers(users);
	}
	
//	@Test
	public void testUpdateUser(){
		User user = new User();
		user.setLogin(11);
		user.setGroup("testgroup");
		user.setEnable(1);
		user.setEnable_change_pass(1);
		user.setEnable_readOnly(0);
		user.setEnableOpt(0);
		user.setPassword_phone("");
		user.setName("testname");
		user.setCity("testcity");
		user.setState("state");
		user.setZipCode("zipcode");
		user.setAddress("address");
		user.setPhone("+86-phone");
		user.setEmail("email@qq.com");
		user.setCountry("country");
		user.setComment("comment");
		user.setId("id");
		user.setState("state");
		user.setRegDate(DateUtil.getTimestamp(new Date()));
		user.setLastDate(DateUtil.getTimestamp(new Date()));
		user.setLeverage(100);
		user.setAgent_account(3284824);
		user.setTimestamp(1497681147);
		user.setBalance(0);
		user.setPrevMonthBalance(0.0);
		user.setPrevBalance(0.0);
		user.setCredit(0.0);
		user.setInterestrate(0.0);
		user.setTaxes(0.0);
		user.setSend_report(1);
		user.setMqid(new BigDecimal("11"));
		user.setUserColor(0);
		user.setEquity(0.0);
		user.setMargin(0.0);
		user.setLead_source("leadsource");
		user.setMargin_free(0.0);
		user.setCurrency("currency");
		user.setStatus("status");
		user.setModify_time(DateUtil.getTimestamp(new Date()));
		userDao.updateUser(user);
	}
	
//	@Test
	public void testUpdateUsers(){
		List<User> users = new ArrayList<>();
		byte[] bytes = new byte[]{1, 0, 0, 0, 1, 1, 0};
		User u1 = new User();
		u1.setLogin(145213678);
		u1.setGroup("updateGroup");
		u1.setEnable(1);
		u1.setEnable_change_pass(1);
		u1.setEnable_readOnly(0);
		u1.setEnableOpt(0);
		u1.setPassword_phone("");
		u1.setName("update");
		u1.setCity("update");
		u1.setState("update");
		u1.setZipCode("update");
		u1.setAddress("update");
		u1.setPhone("+86-update");
		u1.setEmail("update@qq.com");
		u1.setCountry("update");
		u1.setComment("");
		u1.setId("update");
		u1.setState("");
		u1.setRegDate(DateUtil.getTimestamp(new Date()));
		u1.setLastDate(DateUtil.getTimestamp(new Date()));
		u1.setLeverage(100);
		u1.setAgent_account(3284824);
		u1.setTimestamp(1497681147);
		u1.setBalance(0);
		u1.setPrevMonthBalance(0.0);
		u1.setPrevBalance(0.0);
		u1.setCredit(0.0);
		u1.setInterestrate(0.0);
		u1.setTaxes(0.0);
		u1.setSend_report(1);
		u1.setMqid(new BigDecimal("555"));
		u1.setUserColor(0);
		u1.setEquity(0.0);
		u1.setMargin(0.0);
		u1.setLead_source("update");
		u1.setMargin_free(0.0);
		u1.setCurrency("update");
		u1.setStatus("update");
		u1.setApi_data(bytes);
		u1.setLead_source("update");
		u1.setModify_time(DateUtil.getTimestamp(new Date()));
		User u2 = new User();
		u2.setLogin(11);
		u2.setGroup("update");
		u2.setEnable(1);
		u2.setEnable_change_pass(1);
		u2.setEnable_readOnly(0);
		u2.setEnableOpt(0);
		u2.setPassword_phone("");
		u2.setName("update");
		u2.setCity("update");
		u2.setState("update");
		u2.setZipCode("update");
		u2.setAddress("update");
		u2.setPhone("+86-update");
		u2.setEmail("update@qq.com");
		u2.setCountry("update");
		u2.setComment("");
		u2.setId("update");
		u2.setState("");
		u2.setRegDate(DateUtil.getTimestamp(new Date()));
		u2.setLastDate(DateUtil.getTimestamp(new Date()));
		u2.setLeverage(100);
		u2.setAgent_account(3284824);
		u2.setTimestamp(1497681147);
		u2.setBalance(0);
		u2.setPrevMonthBalance(0.0);
		u2.setPrevBalance(0.0);
		u2.setCredit(0.0);
		u2.setInterestrate(0.0);
		u2.setTaxes(0.0);
		u2.setSend_report(1);
		u2.setMqid(new BigDecimal("111"));
		u2.setUserColor(0);
		u2.setEquity(0.0);
		u2.setMargin(0.0);
		u2.setLead_source("update");
		u2.setMargin_free(0.0);
		u2.setCurrency("update");
		u2.setStatus("update");
		u2.setApi_data(bytes);
		u2.setModify_time(DateUtil.getTimestamp(new Date()));
		users.add(u1);
		users.add(u2);
		userDao.updateUsers(users);
	}
	
//	@Test
	public void testDeleteUserByLoginId(){
		userDao.deleteUserByLoginId(7819415);
	}
	
//	@Test
	public void testDeleteUserByLoginIds(){
		List<Integer> ids = new ArrayList<>();
		ids.add(14241);
		ids.add(1234567);
		userDao.deleteUsersByLoginIds(ids);
	}
}
