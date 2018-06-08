package com.broctagon.trad.reportserver.dao.perf;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import javax.swing.ListModel;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.session.ExecutorType;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.trad.reportserver.config.DruidConfig;
import com.broctagon.trad.reportserver.dao.UserDao;
import com.broctagon.trad.reportserver.model.User;
import com.broctagon.trad.reportserver.util.DateUtil;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = {DruidConfig.class})
public class UserDaoPerfTest {
	
	@Autowired
	private UserDao dao;
	
	@Autowired
	private SqlSessionFactory sessionFactory;
	
	private List<User> users;
	
	private User user;
	
	private List<Integer> ids;
	
	@Before
	public void init(){
		users = new ArrayList<>();
		ids = new ArrayList<>();
		for(int i = 0; i < 10000; i ++){
			User user = new User();
			user.setLogin(i);
			user.setGroup("group");
			user.setEnable(0);
			user.setEnable_change_pass(0);
			user.setEnable_readOnly(0);
			user.setEnableOpt(0);
			user.setPassword_phone("pwd_phone");
			user.setName("name");
			user.setCountry("country");
			user.setCity("city");
			user.setState("state");
			user.setZipCode("zipCode");
			user.setAddress("address");
			user.setLead_source("source");
			user.setPhone("phone");
			user.setEmail("email");
			user.setComment("comment");
			user.setId("id");
			user.setStatus("status");
			user.setRegDate(DateUtil.getTimestamp(new Date()));
			user.setLastDate(DateUtil.getTimestamp(new Date()));
			user.setLeverage(1);
			user.setAgent_account(1);
			user.setTimestamp(1);
			user.setBalance(i);
			user.setPrevMonthBalance(i);
			user.setPrevBalance(i);
			user.setCredit(i);
			user.setInterestrate(i);
			user.setTaxes(i);
			user.setSend_report(i);
			user.setMqid(new BigDecimal("1"));
			user.setUserColor(1);
			user.setEquity(i);
			user.setMargin(i);
			user.setMargin_level(i);
			user.setMargin_free(i);
			user.setCurrency("USD");
			user.setApi_data(new byte[]{1, 0, 0, 1});
			user.setModify_time(DateUtil.getTimestamp(new Date()));
			users.add(user);
			ids.add(user.getLogin());
		}
		
		user = new User();
		user.setLogin(-1);
		user.setGroup("group");
		user.setEnable(0);
		user.setEnable_change_pass(0);
		user.setEnable_readOnly(0);
		user.setEnableOpt(0);
		user.setPassword_phone("pwd_phone");
		user.setName("name");
		user.setCountry("country");
		user.setCity("city");
		user.setState("state");
		user.setZipCode("zipCode");
		user.setAddress("address");
		user.setLead_source("source");
		user.setPhone("phone");
		user.setEmail("email");
		user.setComment("comment");
		user.setId("id");
		user.setStatus("status");
		user.setRegDate(DateUtil.getTimestamp(new Date()));
		user.setLastDate(DateUtil.getTimestamp(new Date()));
		user.setLeverage(-1);
		user.setAgent_account(-1);
		user.setTimestamp(-1);
		user.setBalance(-1);
		user.setPrevMonthBalance(-1);
		user.setPrevBalance(-1);
		user.setCredit(-1);
		user.setInterestrate(-1);
		user.setTaxes(-1);
		user.setSend_report(-1);
		user.setMqid(new BigDecimal("1"));
		user.setUserColor(1);
		user.setEquity(-1);
		user.setMargin(-1);
		user.setMargin_level(-1);
		user.setMargin_free(-1);
		user.setCurrency("USD");
		user.setApi_data(new byte[]{1, 0, 0, 1});
		user.setModify_time(DateUtil.getTimestamp(new Date()));
	}
	
//	@Test
	public void testAddOne(){
		long startTime = System.currentTimeMillis();
		dao.addUser(user);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testAddAll(){
		List<User> users1 = users.subList(0, 2000);
		List<User> users2 = users.subList(2001, 4000);
		List<User> users3 = users.subList(4001, 6000);
		List<User> users4 = users.subList(6001, 8000);
		List<User> users5 = users.subList(8001, 9000);
		List<User> users6 = users.subList(9001, 9999);
		long startTime = System.currentTimeMillis();
		dao.addUsers(users1);
		dao.addUsers(users2);
		dao.addUsers(users3);
		dao.addUsers(users4);
		dao.addUsers(users5);
		dao.addUsers(users6);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		// TODO -- It seems only can support 1w in 2s,maybe need to change jdbc template to update 
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testUpdateOne(){
		user.setGroup("update");
		long startTime = System.currentTimeMillis();
		dao.updateUser(user);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
	@Test
	public void testUpdateAll(){
		for(User user: users){
			user.setEmail("test@test.com");
		}
		List<User> users1 = users.subList(0, 2000);
		List<User> users2 = users.subList(2001, 4000);
		List<User> users3 = users.subList(4001, 6000);
		List<User> users4 = users.subList(6001, 8000);
		List<User> users5 = users.subList(8001, 9000);
		List<User> users6 = users.subList(9001, 9999);
		long startTime = System.currentTimeMillis();
		dao.updateUsers(users1);
		dao.updateUsers(users2);
		dao.updateUsers(users3);
		dao.updateUsers(users4);
		dao.updateUsers(users5);
		dao.updateUsers(users6);
		double duration = (System.currentTimeMillis() - startTime) / 1000;
		// TODO -- It seems only can support 1w in 2s,maybe need to change jdbc template to update 
		Assert.assertTrue(duration <= 1);
	}

	
//	@Test
	public void testSelectOne(){
		long startTime = System.currentTimeMillis();
		User user = dao.getUserByLoginId(-1);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		System.out.println(user);
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testSelectAll(){
		long startTime = System.currentTimeMillis();
		List<User> users = dao.getUsersByLoginIds(ids);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testDeleteOne(){
		long startTime = System.currentTimeMillis();
		dao.deleteUserByLoginId(0);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testDeleteAll(){
		long startTime = System.currentTimeMillis();
		dao.deleteUsersByLoginIds(ids);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
}
