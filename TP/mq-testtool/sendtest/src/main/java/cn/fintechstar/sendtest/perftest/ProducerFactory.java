package cn.fintechstar.sendtest.perftest;

import java.io.IOException;

import com.rabbitmq.client.AMQP;
import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ShutdownSignalException;

public class ProducerFactory {
	
	public Producer createProducer(Connection connection, MulticastParams params, Stats stats) throws IOException {
		Channel channel = connection.createChannel();
		if (params.getProducerTxSize() > 0) {
			channel.txSelect();
		}
		if (!exchangeExists(connection, params.getExchangeName())) {
			channel.exchangeDeclare(params.getExchangeName(), params.getExchangeType());
		}
		return new Producer(channel, params, stats);
	}

	private boolean exchangeExists(Connection connection, final String exchangeName) throws IOException {
		return exists(connection, new Checker() {

			public void check(Channel channel) throws IOException {
				channel.exchangeDeclarePassive(exchangeName);
			}

		});
	}

	private static interface Checker {
		public void check(Channel channel) throws IOException;
	}

	private static boolean exists(Connection connection, Checker checker) {
		try {
			Channel channel = connection.createChannel();
			checker.check(channel);
			channel.abort();
			return true;
		} catch (IOException e) {
			ShutdownSignalException shutdownSignalException = (ShutdownSignalException) e.getCause();
			if (!shutdownSignalException.isHardError()) {
				AMQP.Channel.Close close = (AMQP.Channel.Close) shutdownSignalException.getReason();
				if (close.getReplyCode() == AMQP.NOT_FOUND) {
					return false;
				}
			}
			throw new RuntimeException(e);
		}
	}
}
