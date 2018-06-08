package com.broctagon.tradstnode.rabbit.testrm;

import com.broctagon.tradstnode.rabbit.common.Listener;
/**
 * @author imdadullah
 *
 */
public class TestApplication {

	public static void main(String[] args){
		
		Listener.startListener();
	//	Publisher.basicPublishDirect("", 10, "", TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		/*try {

			String filePathValue = null;
			if (args.length > 0) {
				filePathValue = args[0];
			} else {
				filePathValue = "../configs/config.properties";
			}
			System.setProperty("configFilePath", filePathValue);
			ApplicationContext appContext = new ClassPathXmlApplicationContext("bean.xml");
			AmqpTemplate amqpTemplate =  (AmqpTemplate)appContext.getBean("rabbitSender");
			
			Message message = new Message();
			message.setMessageString("HELLO HOW ARE YOU");
			message.setMessageType(TNodeChannelMsgType.TNODE_SERVICE_MSG_TYPE);
			message.setResponceQueue("test.rabbitQ");
			message.setTag((short)10);
			try {
				byte[] body = MessageHelper.prepareMessage(message);
				//message.setContent(MessageHelper.prepareMessage(message));
				MessageProperties messageProperties = new MessageProperties();
	    		messageProperties.setContentLength(body.length);
	    		messageProperties.setDeliveryMode(MessageDeliveryMode.NON_PERSISTENT);
				org.springframework.amqp.core.Message message2 = new org.springframework.amqp.core.Message(body, messageProperties);
				
				amqpTemplate.convertAndSend("t_node_grp", "test.rabbitQ", message2);
				
			} catch (Throwable e) {
				e.printStackTrace();
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}*/

	}
}
