/**
 * 
 */
package com.broctagon.trad.rm.api.model;

import java.util.List;

/**
 * @author imdadullah
 *
 */
public class CandleDataRes {

	String Instrument;
	String Granularity;
	List<Bar> Bars;
	public String getInstrument() {
		return Instrument;
	}
	public void setInstrument(String instrument) {
		Instrument = instrument;
	}
	public String getGranularity() {
		return Granularity;
	}
	public void setGranularity(String granularity) {
		Granularity = granularity;
	}
	public List<Bar> getBars() {
		return Bars;
	}
	public void setBars(List<Bar> bars) {
		Bars = bars;
	}
	
}
