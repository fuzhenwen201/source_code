package cn.fintechstar.sendtest.perftest;

public class ProducerConsumerBase {

	protected float rateLimit;
	protected long lastStatsTime;
	protected int msgCount;
	
	protected void delay(long now){
		long elapsed = now - lastStatsTime;
		long pause = (long) (rateLimit == 0.0f ? 0.0f : (msgCount * 1000.0 / rateLimit - elapsed));
		if (pause > 0) {
			try {
				Thread.sleep(pause);
			} catch (InterruptedException e) {
				throw new RuntimeException(e);
			}
		}
	}
}
