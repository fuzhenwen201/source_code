/**
 * 
 */
package com.broctagon.trad.rm.test;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.src.SpringBootApp;
import com.broctagon.trad.rm.src.message.UserInfo;
import com.broctagon.trad.rm.src.serviceimpl.UpdateUserInfoServiceImp;
import com.broctagon.tradstnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradstnode.rabbit.message.Message;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;

/**
 * @author imdadullah
 *
 */
@RunWith(SpringRunner.class)
@SpringBootTest(classes=SpringBootApp.class)
public class UpdateUserInfoServiceTest {
	
	@Autowired
	private UpdateUserInfoServiceImp updateUserInfoService;
	Message msg;
	
	@Before
	public void userInfo(){
		UserInfo userInfo = new UserInfo();
		userInfo.setReqid("221a9c07734b11e7ae471bcf4ab74159");
		userInfo.setSessionid("e9e11e74734a11e7bf8e835496a721bc");
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
		userInfo.setClientid(3278251);
		userInfo.setMargin(25);
		String userInfoStr = JSON.toJSONString(userInfo);
		msg = new Message();
		msg.setMessageString(userInfoStr);
		
		/*List<UserInfo> userInfoList = new ArrayList<UserInfo>();
		for(int i=1;i<3;i++){
			userInfo.setClientid(3278251);
			userInfo.setMargin(25);
			String userInfoStr = JSON.toJSONString(userInfo);
			byte[] mqBody = prepareEntileData(userInfoStr.getBytes(), CommonConstants.TAG_USERINFO);
			MessageProperties msgProperties = new MessageProperties();
			msgProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);
			msg = new Message();
			msg.
			
			
			userInfoList.add(userInfo);
		}		*/	
	}
	
public byte[] prepareEntileData(byte[] value, short tag){		
		
		com.broctagon.tradstnode.rabbit.message.Message ansMessage = new com.broctagon.tradstnode.rabbit.message.Message();
		ansMessage.setContent(value);
		ansMessage.setTag(tag);
		ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		byte[] body = null;
		try {
			body = MessageHelper.prepareMessage(ansMessage);
		} catch (Throwable e) {
			e.printStackTrace();
		}  
		return body;
	}

	@Test
	public void updateUserInfo(){
		
		updateUserInfoService.writeUserInfoToRedis(msg);
	}

}
