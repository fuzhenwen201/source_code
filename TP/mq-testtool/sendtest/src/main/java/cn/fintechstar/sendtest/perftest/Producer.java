package cn.fintechstar.sendtest.perftest;

import java.io.IOException;

import com.broctagon.tradtnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradtnode.rabbit.message.Message;
import com.broctagon.tradtnode.rabbit.message.MessageHelper;
import com.rabbitmq.client.AMQP.BasicProperties;
import com.rabbitmq.client.Channel;

public class Producer extends ProducerConsumerBase implements Runnable {

	private final Channel channel;
	private final String exchangeName;
	private final String routingKey;
	private final Stats stats;
	private final long timeLimit;
    private final int txSize;
    private final String message;
	private final short tag;

	public Producer(Channel channel, MulticastParams params, Stats stats) {
		this.channel = channel;
		this.exchangeName = params.getExchangeName();
		this.routingKey = params.getRoutingKey();
		this.txSize = params.getProducerTxSize();
		this.rateLimit = params.getProducerRateLimit();
		this.timeLimit = params.getTimeLimit();
		this.message = params.getMessage();
		this.tag = params.getTag();
		this.stats = stats;
	}

	public void run() {
		long now;
		long startTime;
		startTime = now = System.currentTimeMillis();
		lastStatsTime = startTime;
		msgCount = 0;
		int totalMsgCount = 0;
		byte[] body;
		try {
			body = createMessage(message, tag);
			while (timeLimit == 0 || now < startTime + timeLimit) {
				delay(now);
				publish(body);
				totalMsgCount++;
				msgCount++;
				stats.handleSend();
				if (txSize != 0 && totalMsgCount % txSize == 0) {
					channel.txCommit();
				}
				now = System.currentTimeMillis();
			}
		} catch (IOException e) {
			throw new RuntimeException(e);
		} catch (Throwable th) {
			th.printStackTrace();
		}
	}

	private void publish(byte[] body) throws IOException {
		channel.basicPublish(exchangeName, routingKey, new BasicProperties(), body);
	}

	private byte[] createMessage(String message, short tag) throws Throwable {
		Message tnode = new Message();
		tnode.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		tnode.setTag(tag);
		tnode.setMessageString(message);
		return MessageHelper.prepareMessage(tnode);
	}
	

}
