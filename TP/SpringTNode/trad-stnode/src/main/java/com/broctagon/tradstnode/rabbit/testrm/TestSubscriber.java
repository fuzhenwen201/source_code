package com.broctagon.tradstnode.rabbit.testrm;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;

import com.broctagon.tradstnode.rabbit.message.MessageHelper;
import com.broctagon.tradstnode.rabbit.subscribe.Subscriber;

/**
 * @author imdadullah
 *
 */
@Component
@Scope("singleton")
public class TestSubscriber implements Subscriber {

	Logger LOGGER = Logger.getLogger(TestSubscriber.class);

	public void onMessage(Message message) {
		try {
			LOGGER.debug("message=" + message);
			byte[] body = message.getBody();
			com.broctagon.tradstnode.rabbit.message.Message messageRcvd = MessageHelper.parseMessage(body);
			System.out.println("Rcvd Message is >>>>>>>>>>>>>" + messageRcvd.getMessageString());
		} catch (Exception e) {
			
		}
	}

}