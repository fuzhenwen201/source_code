package cn.fintechstar.dcb.model;

public class Candle {

	private String time;
	private double openBid;
	private double openAsk;
	private double highBid;
	private double highAsk;
	private double lowBid;
	private double lowAsk;
	private double closeBid;
	private double closeAsk;
	private int volume;
	private boolean complete;
	private int dirty;		 // 1: dirty data, 2: first come in
	private long startTime;  // candle startTime
	private long endTime;    // candle endTime
	private int count;       // same signal show times
	private InstantSignal preSignal;
	private String granularity;
	
	public String getTime() {
		return time;
	}
	
	public void setTime(String time) {
		this.time = time;
	}
	
	public double getOpenBid() {
		return openBid;
	}
	
	public void setOpenBid(double openBid) {
		this.openBid = openBid;
	}
	
	public double getOpenAsk() {
		return openAsk;
	}
	
	public void setOpenAsk(double openAsk) {
		this.openAsk = openAsk;
	}
	
	public double getHighBid() {
		return highBid;
	}
	
	public void setHighBid(double highBid) {
		this.highBid = highBid;
	}
	
	public double getHighAsk() {
		return highAsk;
	}
	
	public void setHighAsk(double highAsk) {
		this.highAsk = highAsk;
	}
	
	public double getLowBid() {
		return lowBid;
	}
	
	public void setLowBid(double lowBid) {
		this.lowBid = lowBid;
	}
	
	public double getLowAsk() {
		return lowAsk;
	}
	
	public void setLowAsk(double lowAsk) {
		this.lowAsk = lowAsk;
	}
	
	public double getCloseBid() {
		return closeBid;
	}
	
	public void setCloseBid(double closeBid) {
		this.closeBid = closeBid;
	}
	
	public double getCloseAsk() {
		return closeAsk;
	}
	
	public void setCloseAsk(double closeAsk) {
		this.closeAsk = closeAsk;
	}
	
	public int getVolume() {
		return volume;
	}
	
	public void setVolume(int volume) {
		this.volume = volume;
	}
	
	public boolean isComplete() {
		return complete;
	}
	
	public void setComplete(boolean complete) {
		this.complete = complete;
	}

	public int getDirty() {
		return dirty;
	}

	public void setDirty(int dirty) {
		this.dirty = dirty;
	}

	public long getStartTime() {
		return startTime;
	}

	public void setStartTime(long startTime) {
		this.startTime = startTime;
	}

	public long getEndTime() {
		return endTime;
	}

	public void setEndTime(long endTime) {
		this.endTime = endTime;
	}

	public int getCount() {
		return count;
	}

	public void setCount(int count) {
		this.count = count;
	}

	public InstantSignal getPreSignal() {
		return preSignal;
	}

	public void setPreSignal(InstantSignal preSignal) {
		this.preSignal = preSignal;
	}

	public String getGranularity() {
		return granularity;
	}

	public void setGranularity(String granularity) {
		this.granularity = granularity;
	}

	@Override
	public String toString() {
		return "Candle [time=" + time + ", openBid=" + openBid + ", openAsk=" + openAsk + ", highBid=" + highBid
				+ ", highAsk=" + highAsk + ", lowBid=" + lowBid + ", lowAsk=" + lowAsk + ", closeBid=" + closeBid
				+ ", closeAsk=" + closeAsk + ", volume=" + volume + ", complete=" + complete + ", dirty=" + dirty
				+ ", startTime=" + startTime + ", endTime=" + endTime + ", count=" + count + ", preSignal=" + preSignal
				+ ", granularity=" + granularity + "]";
	}

}
