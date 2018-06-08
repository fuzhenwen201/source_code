package cn.fintechstar.dc.constant;

import java.util.HashMap;
import java.util.Map;

public class GranularityConstant {

	
	private static Map<String, Integer> granularityToSecondsMap = new HashMap<>();
	
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
		
	}
	
	public static int getSecondsByGranularity(String granularity){
		return granularityToSecondsMap.get(granularity);
	}

}
