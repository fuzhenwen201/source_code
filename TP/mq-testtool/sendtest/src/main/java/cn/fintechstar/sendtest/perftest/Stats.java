package cn.fintechstar.sendtest.perftest;

public abstract class Stats {
	
	protected final long interval;
	protected final long startTime;
	protected long lastStatsTime;

	protected int sendCountInterval;
	protected int returnCountInterval;
	protected int recvCountInterval;
	
	protected int sendCountTotal;
	protected int recvCountTotal;
	
	protected int latencyCountInterval;
	protected int latencyCountTotal;
	protected long minLatency;
	protected long maxLatency;
	protected long cummulativeLatencyInterval;
	protected long cummulativeLatencyTotal;
	
	protected long elapsedInterval;
	protected long elapsedTotal;
	
	public Stats(long interval){
		this.interval = interval;
		startTime = System.currentTimeMillis();
		reset(startTime);
	}

	private void reset(long time) {
		lastStatsTime = time;
		sendCountInterval = 0;
		returnCountInterval = 0;
		recvCountInterval = 0;
		
		minLatency = Long.MAX_VALUE;
		maxLatency = Long.MIN_VALUE;
		latencyCountInterval = 0;
		cummulativeLatencyInterval = 0L;
	}
	
	protected abstract void report(long now);

	private void report() {
		long now = System.currentTimeMillis();
		elapsedInterval = now - lastStatsTime;
		if (elapsedInterval >= interval) {
			elapsedTotal += elapsedInterval;
			report(now);
			reset(now);
		}
	}
	
	public synchronized void handleSend(){
		sendCountInterval ++;
		sendCountTotal ++;
		report();
	}
	
}
