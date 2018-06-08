package cn.fintechstar.sendtest.perftest;

public class MulticastParams {

	private long confim = -1;
	private int producerCount = 1;
	private int producerChannelCount = 1;
	private int producerTxSize = 0;
	private int channelPrefetch = 0;
	private int minMsgSize = 0;

	private int timeLimit = 0;
	private float producerRateLimit = 0;
	private int producerMsgCount = 0;

	private String exchangeName = "testool";
	private String exchangeType = "topic";
	private String queueName = "testool";
	private String routingKey = null;
	
	private String message;
	private short tag;

	private boolean autoAck = false;
	private boolean autoDelete = true;


	public long getConfim() {
		return confim;
	}

	public void setConfim(long confim) {
		this.confim = confim;
	}

	public int getProducerCount() {
		return producerCount;
	}

	public void setProducerCount(int producerCount) {
		this.producerCount = producerCount;
	}

	public int getProducerChannelCount() {
		return producerChannelCount;
	}

	public void setProducerChannelCount(int producerChannelCount) {
		this.producerChannelCount = producerChannelCount;
	}

	public int getProducerTxSize() {
		return producerTxSize;
	}

	public void setProducerTxSize(int producerTxSize) {
		this.producerTxSize = producerTxSize;
	}

	public int getChannelPrefetch() {
		return channelPrefetch;
	}

	public void setChannelPrefetch(int channelPrefetch) {
		this.channelPrefetch = channelPrefetch;
	}

	public int getMinMsgSize() {
		return minMsgSize;
	}

	public void setMinMsgSize(int minMsgSize) {
		this.minMsgSize = minMsgSize;
	}

	public int getTimeLimit() {
		return timeLimit;
	}

	public void setTimeLimit(int timeLimit) {
		this.timeLimit = timeLimit;
	}

	public float getProducerRateLimit() {
		return producerRateLimit;
	}

	public void setProducerRateLimit(float producerRateLimit) {
		this.producerRateLimit = producerRateLimit;
	}

	public int getProducerMsgCount() {
		return producerMsgCount;
	}

	public void setProducerMsgCount(int producerMsgCount) {
		this.producerMsgCount = producerMsgCount;
	}

	public String getExchangeName() {
		return exchangeName;
	}

	public void setExchangeName(String exchangeName) {
		this.exchangeName = exchangeName;
	}

	public String getExchangeType() {
		return exchangeType;
	}

	public void setExchangeType(String exchangeType) {
		this.exchangeType = exchangeType;
	}

	public String getQueueName() {
		return queueName;
	}

	public void setQueueName(String queueName) {
		this.queueName = queueName;
	}

	public String getRoutingKey() {
		return routingKey;
	}

	public void setRoutingKey(String routingKey) {
		this.routingKey = routingKey;
	}

	public String getMessage() {
		return message;
	}

	public void setMessage(String message) {
		this.message = message;
	}

	public short getTag() {
		return tag;
	}

	public void setTag(short tag) {
		this.tag = tag;
	}

	public boolean isAutoAck() {
		return autoAck;
	}

	public void setAutoAck(boolean autoAck) {
		this.autoAck = autoAck;
	}

	public boolean isAutoDelete() {
		return autoDelete;
	}

	public void setAutoDelete(boolean autoDelete) {
		this.autoDelete = autoDelete;
	}


}
