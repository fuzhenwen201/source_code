package cn.fintechstar.traddc.constant;

import java.util.HashMap;
import java.util.Map;

import cn.fintechstar.traddc.model.Granularity;

public class GranularityConstant {

	
	private static Map<Granularity, Integer> granularityToSecondsMap = new HashMap<>();
	
	private static Map<Granularity, Integer> secondsMap = new HashMap<>();
	
	private static Map<Granularity, Integer> minutesMap = new HashMap<>();
	
	private static Map<Granularity, Integer> hoursMap = new HashMap<>();
	
	static{
		granularityToSecondsMap.put(Granularity.S5,  5);
		granularityToSecondsMap.put(Granularity.S10, 10);
		granularityToSecondsMap.put(Granularity.S15, 15);
		granularityToSecondsMap.put(Granularity.S30, 30);
		granularityToSecondsMap.put(Granularity.M1,  60);
		granularityToSecondsMap.put(Granularity.M5,  5  * 60);
		granularityToSecondsMap.put(Granularity.M10, 10 * 60);
		granularityToSecondsMap.put(Granularity.M15, 15 * 60);
		granularityToSecondsMap.put(Granularity.M30, 30 * 60);
		granularityToSecondsMap.put(Granularity.H1,  1  * 60 * 60);
		granularityToSecondsMap.put(Granularity.H4,  4  * 60 * 60);
		granularityToSecondsMap.put(Granularity.H12, 12 * 60 * 60);
		granularityToSecondsMap.put(Granularity.D,   1  * 24 * 60 * 60);
		granularityToSecondsMap.put(Granularity.W,   1  * 7  * 24 * 60 * 60);
		
		secondsMap.put(Granularity.S5, 5);
		secondsMap.put(Granularity.S10, 10);
		secondsMap.put(Granularity.S15, 15);
		secondsMap.put(Granularity.S30, 30);
		
		minutesMap.put(Granularity.M1, 1);
		minutesMap.put(Granularity.M5, 5);
		minutesMap.put(Granularity.M10, 10);
		minutesMap.put(Granularity.M15, 15);
		minutesMap.put(Granularity.M30, 30);
		
		hoursMap.put(Granularity.H1, 1);
		hoursMap.put(Granularity.H4, 4);
		hoursMap.put(Granularity.H12, 12);
	}
	
	public static int getSecondsByGranularity(Granularity granularity){
		return granularityToSecondsMap.get(granularity);
	}

	public static Map<Granularity, Integer> getSecondsMap() {
		return secondsMap;
	}

	public static Map<Granularity, Integer> getMinutesMap() {
		return minutesMap;
	}

	public static Map<Granularity, Integer> getHoursMap() {
		return hoursMap;
	}


}
