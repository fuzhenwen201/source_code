package com.broctagon.trad.rm.test.serviceimpl;

import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageDeliveryMode;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.src.common.CommonConstants;
import com.broctagon.trad.rm.src.message.UserInfo;
import com.broctagon.tradstnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;

@Component
public class MQSource{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private RabbitTemplate userInfoSender;
/*	@Autowired
	private RabbitTemplate positionSender;*/
	
	public void SendUserInfoMQ(){
				
//		{"reqid":"221a9c07734b11e7ae471bcf4ab74159","sessionid":"e9e11e74734a11e7bf8e835496a721bc","groupid":0,
//		"clientid":1,"accountid":0,"prevbalance":0,"changed":10000000,"margin":0,"freemargin":9999540,
//		"profit":-460,"type":128,"closebalance":0,"status":1,"errmsg":"success"}
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
		
		List<UserInfo> userInfoList = new ArrayList<UserInfo>();
		for(int i=1;i<3;i++){
			userInfo.setClientid(3278251);
			userInfo.setMargin(25);
			String userInfoStr = JSON.toJSONString(userInfo);
			logger.info("userInfoStr:"+userInfoStr);
			byte[] mqBody = prepareEntileData(userInfoStr.getBytes(), CommonConstants.TAG_USERINFO);
			MessageProperties msgProperties = new MessageProperties();
			msgProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);
			Message msg = new Message(mqBody, msgProperties);
			
			userInfoSender.send(msg);
			
			userInfoList.add(userInfo);
		}			
	}
	
	/*	{"reqid":"4e978baf75d54a5d8bca9c0491f9ff9d","groupid":1,"clientid":82,"orderid":"22922d0e631e11e7aaf7b64ac1597823","orderstatus":0,
	"assetid":1,"direction":0,"amount":100,"expiryperiod":0,"opentime":1499436673826456,"openprice":729000,"closeprice":0,"payout":0,
	"sgid":1,"teid":0,"expirytime":"1499436673871456"
}*/

/*	{"sessionid":"55e23d1a737811e7bf8eb845127a3c96","reqid":"9f487ccf972f41e78b4477abba28b982","groupid":0,"clientid":75,
	"orderid":"5a37d820737811e7bf8ec9b8a6734251","orderstatus":18,"assetid":0,"direction":1,"amount":100,"expiryperiod":5,
	"opentime":0,"openprice":0,"closeprice":0,"payout":0,"sgid":1011,"teid":21}*/
	
	/*public void SendPositionMQ(){
		Order order = new Order();
		order.setReqid("4e978baf75d54a5d8bca9c0491f9ff9d");
		order.setGroupid(1);
		order.setClientid(82);
		order.setOrderstatus(0);
		order.setAssetid(1);
		order.setDirection(0);
		order.setAmount(100);
		int expiryperiod = 5;
		order.setExpiryperiod(expiryperiod);
		long opentime = System.currentTimeMillis() * 1000;
		order.setOpentime(opentime);
		order.setOpenprice(729000);
		order.setPayout(0);
		order.setSgid(1);
		order.setTeid(0);
		order.setExpirytime(opentime+ expiryperiod*1000*1000);		

		for(int i=0; i<5; i++){
			
			if(i%2==0){
				order.setCloseprice(0);
			}
			else{
				order.setCloseprice(729002);
			}
			
			UUID uuid = UUID.randomUUID();
			String uuidStr = uuid.toString().replaceAll("-", "");	
			order.setOrderid(uuidStr);
			String orderStr = JSON.toJSONString(order);
			logger.info("orderStr:"+orderStr);
			
			byte[] body = prepareEntileData(orderStr.getBytes(), CommonConstants.TAG_ORDERANS);
			MessageProperties msgProperties = new MessageProperties();
			msgProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);
			Message msg = new Message(body, msgProperties);
			positionSender.send(msg);
		}

		
	}*/
	
	
	
	public byte[] prepareEntileData(byte[] value, short tag){		
		
		com.broctagon.tradstnode.rabbit.message.Message ansMessage = new com.broctagon.tradstnode.rabbit.message.Message();
		ansMessage.setContent(value);
		ansMessage.setTag(tag);
		ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		byte[] body = null;
		try {
			body = MessageHelper.prepareMessage(ansMessage);
		} catch (Throwable e) {
			logger.error(e.getMessage());
			e.printStackTrace();
		}  
		return body;
	}
	
}
