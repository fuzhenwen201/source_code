/**
 * 
 */
package com.broctagon.trad.rm.test;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.src.SpringBootApp;
import com.broctagon.trad.rm.src.message.UserInfo;
import com.broctagon.trad.rm.src.service.OnlineUserSetService;

/**
 * @author imdadullah
 *
 */

@RunWith(SpringRunner.class)
@SpringBootTest(classes=SpringBootApp.class)
public class OnlineUserSetServiceTest {

	@Autowired
	private OnlineUserSetService onlineUserSetService;
	
	public String userInfo(){
		UserInfo userInfo = new UserInfo();
		userInfo.setReqid("221a9c07734b11e7ae471bcf4ab74159");
		userInfo.setSessionid("e9e11e74734a11e7bf8e835496a721bc");
		userInfo.setClientid(3);
		userInfo.setMargin(25);
		userInfo.setGroupid(0);	
		userInfo.setAccountid(0);
		userInfo.setPrevbalance(0);
		userInfo.setChanged(10000000);
		userInfo.setMargin(0);
		userInfo.setFreemargin(9999540);
		userInfo.setProfit(-460);
		userInfo.setType(128);
		userInfo.setClosebalance(0);
		userInfo.setStatus(1);
		userInfo.setErrmsg("success");	
		String userInfoStr = JSON.toJSONString(userInfo);
		System.out.println("User Info String is >>>>>>>>" +userInfoStr);
		return userInfoStr;
	}
	
	@Test
	public void onlineUserTest(){
		onlineUserSetService.addOnlineUser(userInfo().getBytes());
		onlineUserSetService.removeOnlineUser(userInfo().getBytes());
		onlineUserSetService.requestAllOnlineUsers();
		//onlineUserSetService.storeAllOnlineUsers(userInfo().getBytes());
		
	}
}
