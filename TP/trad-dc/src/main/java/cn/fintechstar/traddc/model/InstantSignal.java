package cn.fintechstar.traddc.model;

public class InstantSignal {

	private Instrument instrument;
	private int status;
	private int bid;
	private int ask;
	private long timestamp;
	
	public InstantSignal(){
		
	}
	
	public InstantSignal(Instrument instrument, int status, int bid, int ask, long timestamp){
		this.instrument = instrument;
		this.status = status;
		this.bid = bid;
		this.ask = ask;
		this.timestamp = timestamp;
	}
	
	
	public Instrument getInstrument() {
		return instrument;
	}
	
	public void setInstrument(Instrument instrument) {
		this.instrument = instrument;
	}
	
	public int getStatus() {
		return status;
	}
	
	public void setStatus(int status) {
		this.status = status;
	}
	
	public int getBid() {
		return bid;
	}
	
	public void setBid(int bid) {
		this.bid = bid;
	}
	
	public int getAsk() {
		return ask;
	}
	
	public void setAsk(int ask) {
		this.ask = ask;
	}
	
	public long getTimestamp() {
		return timestamp;
	}
	
	public void setTimestamp(long timestamp) {
		this.timestamp = timestamp;
	}

	@Override
	public String toString() {
		return "InstantSignal [instrument=" + instrument + ", status=" + status + ", bid=" + bid + ", ask=" + ask
				+ ", timestamp=" + timestamp + "]";
	}

}
