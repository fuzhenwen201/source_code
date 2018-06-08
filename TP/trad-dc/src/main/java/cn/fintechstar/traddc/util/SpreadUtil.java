package cn.fintechstar.traddc.util;

public class SpreadUtil {
	
	public static double calculateAskSpread(double ask, int spread) {
		return ask + spread / 2;
	}
	
	public static double calculateBidSpread(double bid, int spread) {
		if(spread % 2 != 0){
			spread = spread + 1;
		}
		return bid - spread / 2;
	}
}
