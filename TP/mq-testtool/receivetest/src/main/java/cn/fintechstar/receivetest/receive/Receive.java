package cn.fintechstar.receivetest.receive;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Service;

@Service
public class Receive {
	
	public static long count = 0;
	
	@RabbitListener(queues="DATAREQANS.100")
	public void receiveFromAns(Message message){
		count ++;
	}
}
