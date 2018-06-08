package cn.fintechstar.dcb.util;

public class CommonUtil {
	
	private static final int NUMBER_FORMAT = 1000000;
	
	public static String getHistoryKey(String instrument, String granularity) {
		return instrument + "_" + granularity;
	}
	
	public static double objToDoubleByNumberFormat(Object obj){
		return (double)Integer.parseInt(obj.toString()) / NUMBER_FORMAT;
	}
	
	public static double intToDoubleByNumberFormat(int i){
		return (double)i / NUMBER_FORMAT;
	}
	
	public static double doubleToDoubleByNumberFormat(double d){
		return d / NUMBER_FORMAT;
	}
}
