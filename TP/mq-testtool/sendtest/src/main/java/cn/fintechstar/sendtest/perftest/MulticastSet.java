package cn.fintechstar.sendtest.perftest;

import java.io.IOException;
import java.net.URISyntaxException;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.util.concurrent.TimeoutException;

import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;

public class MulticastSet {

	private final Stats stats;
	private final ConnectionFactory connectionFactory;
	private final MulticastParams params;
	private final String testId;
	private final String host;
	
	public MulticastSet(Stats stats, ConnectionFactory factory, MulticastParams params, String host){
		this.connectionFactory = factory;
		this.params = params;
		this.testId = "perftest";
		this.host = host;
		this.stats = stats;
	}

	public void run(boolean announceStartup) throws KeyManagementException, NoSuchAlgorithmException, URISyntaxException, IOException, TimeoutException, InterruptedException {
		Thread[] producerThreads = new Thread[params.getProducerCount()];
		Connection[] producerConnections = new Connection[params.getProducerCount()];
		for(int i = 0; i < producerConnections.length; i++){
			if (announceStartup) {
				System.out.println("id : " + testId + ", starting producer #" + i);
			}
			setUri();
			Connection connection = connectionFactory.newConnection();
			producerConnections[i] = connection;
			for(int j = 0; j < params.getProducerChannelCount(); j ++){
				if (announceStartup) {
					System.out.println("id : " + testId + ", starting producer #" + i + ", channel #" + j);
				}
				Thread thread = new Thread(new ProducerFactory().createProducer(connection, params, stats));
				producerThreads[(i * params.getProducerChannelCount()) + j] = thread;
			}
		}
		
		for(Thread producer : producerThreads){
			producer.start();
		}	

		int count = 1;
		for(int i = 0; i < producerThreads.length; i++){
			producerThreads[i].join();
			if (count % params.getProducerChannelCount() == 0) {
				producerConnections[count / params.getProducerChannelCount() - 1].close();
			}
			count++;
		}
	}

	private void setUri() throws KeyManagementException, NoSuchAlgorithmException, URISyntaxException {
		connectionFactory.setUri(host);
	}
	
}
