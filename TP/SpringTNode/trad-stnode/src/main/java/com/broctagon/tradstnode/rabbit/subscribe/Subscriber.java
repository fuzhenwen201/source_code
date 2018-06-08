/**
 * 
 */
package com.broctagon.tradstnode.rabbit.subscribe;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageListener;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;

/**
 * @author imdadullah
 *
 */
@Component
@Scope("singleton")
public interface Subscriber extends MessageListener {
	
	public void onMessage(Message message);
	
}
