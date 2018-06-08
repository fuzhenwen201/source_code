/**
 * 
 */
package com.broctagon.trad.rm.obj.rabbitmq;

import java.util.Date;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;

import com.broctagon.trad.rm.obj.common.RMLogger;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;
import com.broctagon.tradstnode.rabbit.subscribe.Subscriber;
import com.broctagon.tradstnode.rabbit.utils.ConstantUtils;
import com.broctagon.tradstnode.rabbit.utils.DateUtils;




/**
 * @author imdadullah
 *
 */
@Component
@Scope(value = "singleton")
public class RMListener implements Subscriber {
	private Logger LOGGER = Logger.getLogger(this.getClass());

	@Autowired
	private AnalysMessage analysMessage;

	@Override
	public void onMessage(Message message) {
		try {
			LOGGER.debug("message=" + message);
			byte[] body = message.getBody();
			com.broctagon.tradstnode.rabbit.message.Message messageRcvd = MessageHelper.parseMessage(body);
			LOGGER.info("@@@@@@@@@@@@ ON EVENT MESSAGE RECEIVED BY RMLISTENER IS =====>>> TAG IS [" + messageRcvd.getTag() + "]" + messageRcvd.getMessageString());
			RMLogger.writeLogMessage(DateUtils.convertDateToString(new Date(), DateUtils.DdMmYyyyHmsFormatWithZone),
					ConstantUtils.MESSAGE_IN, messageRcvd.getTag(), messageRcvd.getMessageString().length(),messageRcvd.getMessageString());
			analysMessage.checkMessage(messageRcvd);
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}

	}

}
