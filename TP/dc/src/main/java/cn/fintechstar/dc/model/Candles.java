package cn.fintechstar.dc.model;

import java.util.List;

public class Candles {

	private String instrument;
	
	private String granularity;
	
	private List<Candle> candleList;
	
	
	public String getInstrument() {
		return instrument;
	}
	
	public void setInstrument(String instrument) {
		this.instrument = instrument;
	}
	
	public String getGranularity() {
		return granularity;
	}
	
	public void setGranularity(String granularity) {
		this.granularity = granularity;
	}
	
	public List<Candle> getCandleList() {
		return candleList;
	}
	
	public void setCandleList(List<Candle> candleList) {
		this.candleList = candleList;
	}

	@Override
	public String toString() {
		return "Candles [instrument=" + instrument + ", granularity=" + granularity + ", candleList=" + candleList + "]";
	}

}
