package com.broctagon.trad.reportserver.serviceimpl;

import java.math.BigDecimal;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.reportserver.dao.UserDao;
import com.broctagon.trad.reportserver.model.BalanceTrader;
import com.broctagon.trad.reportserver.model.BalanceUser;
import com.broctagon.trad.reportserver.model.Trader;
import com.broctagon.trad.reportserver.model.User;
import com.broctagon.trad.reportserver.service.MT4UserService;


@Service("userService")
public class MT4UserServiceImpl implements MT4UserService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private UserDao userDao;
	
	public void updateBalanceUsers(List<String> traderStrList) {

		List<BalanceUser> userList = getMT4BalanceUsers(traderStrList);
		if (userList == null || userList.size() == 0) {
			logger.error("userList==null||userList.size()==0");
			return;
		}
		logger.info("userList:" + JSON.toJSONString(userList));
		try {
			for (BalanceUser user : userList) {
				userDao.updateBalanceUser(user);
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}
			
	public void addMT4Users(List<String> traderStrList) {
		List<User> userList = getMT4UsersListBySouce(traderStrList);
		if (userList == null || userList.size() == 0) {
			logger.error(userList == null || userList.size() == 0);
			return;
		}
		try {
			userDao.addUsers(userList);
		} catch (Exception ex) {
			ex.printStackTrace();
		}

	}
	
	public List<User> getMT4UsersListBySouce(List<String> traderStrList){		
		List<User> userlist = new ArrayList<User>();
	//	logger.info("traderStrList.size():"+traderStrList.size());
		Timestamp testTimestamp = Timestamp.valueOf("2017-07-04 15:36:00");
		long currentTimeMillis = System.currentTimeMillis();	
		Timestamp currrentTimestamp = new Timestamp(currentTimeMillis);
		
		for(int i=0; i<traderStrList.size(); i++){
			String traderStr = traderStrList.get(i);
			Trader trader = JSON.parseObject(traderStr, Trader.class);
			
			User user = new User();
			user.setLogin(trader.getClientid());
			user.setGroup(trader.getGroupname());
			user.setEnable(1);  
			user.setEnable_change_pass(1);  
			user.setEnable_readOnly(0);    
			user.setEnableOpt(0);
			user.setPassword_phone("");   
			user.setName(trader.getFirstname()+" "+trader.getLastname());
			if(trader.getCountry()!=null){
				user.setCountry(trader.getCountry());
			}
			else{
				logger.info("trader.getCountry()==null");
				user.setCountry("country");
			}
			String city = trader.getCity();
			if(city.length()>32){
				city=city.substring(0, 32);
			}
			user.setCity(city);
			String status = trader.getState();
			
			if(status!=null){
				if(status.length()>32){
					status=status.substring(0, 32);
				}
				user.setState(status);
			}
			else{
				logger.info("trader.getState()==null");
				user.setState("sta");
			}
			
			String pin = trader.getPin();	
			if(pin.length()>16){
				pin=pin.substring(0, 16);
			}
			user.setZipCode(pin);
			
			user.setAddress(trader.getAddress());
			user.setLead_source("leaderSourceTest");
			user.setPhone("18221071660");       
			
			String email = trader.getEmail();
			if(email.length()>48){
				email = email.substring(0, 48);
			}
			user.setEmail(email);   
			
			user.setComment("");
			user.setId("");   // 
			user.setStatus("");		
			user.setRegDate(testTimestamp);
			user.setLastDate(currrentTimestamp);
			user.setLeverage(0);
			user.setAgent_account(0);  //
			user.setTimestamp(1497681147);
			user.setBalance(trader.getBalance()/1000.0);
			user.setPrevMonthBalance(trader.getPrevbalance()/1000.0);  //
			user.setPrevBalance(trader.getPrevbalance()/1000.0);
			user.setCredit(0);
			user.setInterestrate(0);
			user.setTaxes(0);
			user.setSend_report(1);
			user.setMqid(new BigDecimal("1"));  
			user.setUserColor(0);
			user.setEquity(0);
			user.setMargin(trader.getMargin()/1000.0);
			user.setMargin_level(0);
			user.setMargin_free(trader.getFreemargin()/1000.0);
			user.setCurrency("");
			user.setApi_data(new byte[]{});
			user.setModify_time(currrentTimestamp);
			userlist.add(user);
		}
					
		return userlist;
	}
	
	public List<BalanceUser> getMT4BalanceUsers(List<String> traderStrList){		
		List<BalanceUser> userlist = new ArrayList<BalanceUser>();
		logger.info("traderStrList.size():"+traderStrList.size());		
		for(int i=0; i<traderStrList.size(); i++){
			String traderStr = traderStrList.get(i);
			BalanceTrader trader = JSON.parseObject(traderStr, BalanceTrader.class);		
			BalanceUser user = new BalanceUser();
			user.setBalance(trader.getBalance()/1000.0);
			user.setLogin(trader.getClientid());
			user.setMargin_free(trader.getFreemargin()/1000.0);
			user.setMargin(trader.getMargin()/1000.0);
			user.setPrevBalance(trader.getPrevbalance()/1000.0);
			userlist.add(user);
		}					
		return userlist;
	}
		
}
