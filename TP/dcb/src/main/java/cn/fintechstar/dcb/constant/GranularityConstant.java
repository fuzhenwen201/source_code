package cn.fintechstar.dcb.constant;

import java.util.HashMap;
import java.util.Map;

public class GranularityConstant {

	private static Map<String, Integer> granularityToSecondsMap = new HashMap<>();
	
	private static Map<String, Integer> secondsMap = new HashMap<>();
	
	private static Map<String, Integer> minutesMap = new HashMap<>();
	
	private static Map<String, Integer> hoursMap = new HashMap<>();
	
	static{
		granularityToSecondsMap.put("S5",  5);
		granularityToSecondsMap.put("S10", 10);
		granularityToSecondsMap.put("S15", 15);
		granularityToSecondsMap.put("S30", 30);
		granularityToSecondsMap.put("M1",  60);
		granularityToSecondsMap.put("M5",  5  * 60);
		granularityToSecondsMap.put("M10", 10 * 60);
		granularityToSecondsMap.put("M15", 15 * 60);
		granularityToSecondsMap.put("M30", 30 * 60);
		granularityToSecondsMap.put("H1",  1  * 60 * 60);
		granularityToSecondsMap.put("H4",  4  * 60 * 60);
		granularityToSecondsMap.put("H12", 12 * 60 * 60);
		granularityToSecondsMap.put("D",   1  * 24 * 60 * 60);
		granularityToSecondsMap.put("W",   1  * 7  * 24 * 60 * 60);
		granularityToSecondsMap.put("M",   0);
		granularityToSecondsMap.put("NULL_VAL", 1);
		
		secondsMap.put("S5", 5);
		secondsMap.put("S10", 10);
		secondsMap.put("S15", 15);
		secondsMap.put("S30", 30);
		secondsMap.put("NULL_VAL", 1);
		
		minutesMap.put("M1", 1);
		minutesMap.put("M5", 5);
		minutesMap.put("M10", 10);
		minutesMap.put("M15", 15);
		minutesMap.put("M30", 30);
		
		hoursMap.put("H1", 1);
		hoursMap.put("H4", 4);
		hoursMap.put("H12", 12);
	}


	public static Map<String, Integer> getGranularityToSecondsMap() {
		return granularityToSecondsMap;
	}

	public static Map<String, Integer> getSecondsMap() {
		return secondsMap;
	}

	public static Map<String, Integer> getMinutesMap() {
		return minutesMap;
	}

	public static Map<String, Integer> getHoursMap() {
		return hoursMap;
	}


}
