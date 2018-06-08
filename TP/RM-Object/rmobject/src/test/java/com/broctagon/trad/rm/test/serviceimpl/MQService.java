/**
 * 
 */
package com.broctagon.trad.rm.test.serviceimpl;

import java.util.UUID;

import org.apache.log4j.Logger;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageDeliveryMode;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.SpringBootApp;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.tradstnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;

/**
 * @author imdadullah
 *
 */
@RunWith(SpringRunner.class)
@SpringBootTest(classes=SpringBootApp.class)
public class MQService {
	private Logger logger = Logger.getLogger(this.getClass());
	@Autowired
	private RabbitTemplate positionSender;
	
	@Test
	public void SendPositionToMQ(){
	TradeModel model = new TradeModel();
	model.setReqid("4e978baf75d54a5d8bca9c0491f9ff9d");
	model.setGroupid(1);
	model.setClientid(82);
	model.setOrderstatus(0);
	model.setAssetid(1);
	model.setDirection(0);
	model.setAmount(100);
	int expiryperiod = 5;
	model.setExpiryperiod(expiryperiod);
	long opentime = System.currentTimeMillis() * 1000;
	model.setOpentime(opentime);
	model.setOpenprice(729000);
	model.setPayout(0);
	model.setSgid(1);
	model.setTeid(0);
	model.setExpirytime(opentime+ expiryperiod*1000*1000);		

	for(int i=0; i<5; i++){
		
		if(i%2==0){
			model.setCloseprice(0);
		}
		else{
			model.setCloseprice(729002);
		}
		
		UUID uuid = UUID.randomUUID();
		String uuidStr = uuid.toString().replaceAll("-", "");	
		model.setOrderid(uuidStr);
		String orderStr = JSON.toJSONString(model);
		logger.info("orderStr:"+orderStr);
		
		byte[] body = prepareEntileData(orderStr.getBytes(), CommonConstants.MESSAGE_TAG_ORDERANS_RM);
		MessageProperties msgProperties = new MessageProperties();
		msgProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);
		Message msg = new Message(body, msgProperties);
		positionSender.send(msg);
	}

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
			logger.error(e.getMessage());
			e.printStackTrace();
		}  
		return body;
	}

}
