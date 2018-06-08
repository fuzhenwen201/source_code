package cn.fintechstar.dc.util;

public class CommonUtil {
	
	public static String getHistoryKey(String instrument, String granularity) {
		return instrument + "_" + granularity;
	}

}
