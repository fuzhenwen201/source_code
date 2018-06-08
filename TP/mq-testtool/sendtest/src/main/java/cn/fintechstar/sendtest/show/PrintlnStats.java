package cn.fintechstar.sendtest.show;

import cn.fintechstar.sendtest.perftest.Stats;

public class PrintlnStats extends Stats{

	private final String testId;	
	
	public PrintlnStats(String testId, long interval){
		super(interval);
		this.testId = testId;
	}

	@Override
	protected void report(long now) {
		StringBuilder output = new StringBuilder("id : " + testId +" - ");
		output.append("time : " + String.format("%.3f", (now - startTime)/1000.0) + "s")
			  .append(getRate("sent", sendCountInterval, elapsedInterval));
		if (latencyCountInterval > 0) {
			output.append(", min/avg/max latency : " + minLatency/1000L + "/" + cummulativeLatencyInterval / (1000 * latencyCountInterval) + "/" + maxLatency / 1000 + " microseconds");
		}
		System.out.println(output);
	}

	private String getRate(String descrip, long count, long elapsedInterval) {
		return ", " + descrip + ": " + formatRate(1000.0 * count /elapsedInterval) + "msg/s";
	}
	
    private static String formatRate(double rate) {
        if (rate == 0.0)    return String.format("%d", (long)rate);
        else if (rate < 1)  return String.format("%1.2f", rate);
        else if (rate < 10) return String.format("%1.1f", rate);
        else                return String.format("%d", (long)rate);
    }

	public void print() {
		long now = System.currentTimeMillis();
		System.out.println("id : " + testId + ", sending rate avg : " + formatRate(sendCountTotal * 1000 / (now  - startTime)) + "msg/s");
	}
}
