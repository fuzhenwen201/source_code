package cn.fintechstar.traddc.model;

import java.util.List;

public class Candles {

	private Instrument instrument;
	private Granularity granularity;
	private List<Candle> candleList;

	public Instrument getInstrument() {
		return instrument;
	}

	public void setInstrument(Instrument instrument) {
		this.instrument = instrument;
	}

	public Granularity getGranularity() {
		return granularity;
	}

	public void setGranularity(Granularity granularity) {
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
		return "Candles{" +
				"instrument=" + instrument +
				", granularity=" + granularity +
				", candleList=" + candleList +
				'}';
	}
}
