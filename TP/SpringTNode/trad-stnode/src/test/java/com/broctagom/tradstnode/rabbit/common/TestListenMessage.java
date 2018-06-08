/**
 * 
 */
package com.broctagom.tradstnode.rabbit.common;



import org.springframework.amqp.core.MessageListener;

import com.broctagon.tradstnode.rabbit.message.Message;

/**
 * @author imdadullah
 *
 */
public interface TestListenMessage extends MessageListener{
	
	void onMessage(Message message);
}
