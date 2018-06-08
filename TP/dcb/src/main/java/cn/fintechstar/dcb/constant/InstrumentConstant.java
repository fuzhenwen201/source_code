package cn.fintechstar.dcb.constant;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentLinkedQueue;

import cn.fintechstar.dcb.model.InstantSignal;

public class InstrumentConstant {

	private static Map<Integer, ConcurrentLinkedQueue<InstantSignal>> INSTRUMENT_TO_QUEUE = new HashMap<>();
	
	private static Map<String, Integer> stringToInt = new HashMap<>();
	
	private static Map<Integer, String> intToString = new HashMap<>();
	
	static{
		INSTRUMENT_TO_QUEUE.put(0, SignalConstant.GROUP_0);
		INSTRUMENT_TO_QUEUE.put(1, SignalConstant.GROUP_1);
		INSTRUMENT_TO_QUEUE.put(2, SignalConstant.GROUP_2);
		INSTRUMENT_TO_QUEUE.put(3, SignalConstant.GROUP_3);
		INSTRUMENT_TO_QUEUE.put(4, SignalConstant.GROUP_4);
		INSTRUMENT_TO_QUEUE.put(5, SignalConstant.GROUP_5);

		
		stringToInt.put("AUDCAD", 0);
		stringToInt.put("AUDSGD", 1);
		stringToInt.put("CADSGD", 2);
		stringToInt.put("EURCAD", 3);
		stringToInt.put("GBPJPY", 4);
		stringToInt.put("NZDJPY", 5);
		stringToInt.put("SGDJPY", 6);
		stringToInt.put("USDJPY", 7);
		stringToInt.put("ZARJPY", 8);
		stringToInt.put("AUDCHF", 9);
		stringToInt.put("AUDUSD", 10);
		stringToInt.put("EURCHF", 11);
		stringToInt.put("GBPAUD", 12);
		stringToInt.put("GBPNZD", 13);
		stringToInt.put("TRYJPY", 14);
		stringToInt.put("USDSGD", 15);
		stringToInt.put("CADCHF", 16);
		stringToInt.put("CHFJPY", 17);
		stringToInt.put("EURJPY", 18);
		stringToInt.put("EURSGD", 19);
		stringToInt.put("GBPCAD", 20);
		stringToInt.put("NZDCAD", 21);
		stringToInt.put("NZDUSD", 22);
		stringToInt.put("USDCAD", 23);
		stringToInt.put("USDHKD", 24);
		stringToInt.put("AUDJPY", 25);
		stringToInt.put("EURTRY", 26);
		stringToInt.put("GBPCHF", 27);
		stringToInt.put("GBPSGD", 28);
		stringToInt.put("NZDCHF", 29);
		stringToInt.put("USDCHF", 30);
		stringToInt.put("USDTRY", 31);
		stringToInt.put("AUDNZD", 32);
		stringToInt.put("CADJPY", 33);
		stringToInt.put("EURAUD", 34);
		stringToInt.put("EURGBP", 35);
		stringToInt.put("EURNZD", 36);
		stringToInt.put("EURUSD", 37);
		stringToInt.put("GBPUSD", 38);
		stringToInt.put("USDCNH", 39);
		stringToInt.put("USDINR", 40);
		
		intToString.put(0, "AUDCAD");
		intToString.put(1, "AUDSGD");
		intToString.put(2, "CADSGD");
		intToString.put(3, "EURCAD");
		intToString.put(4, "GBPJPY");
		intToString.put(5, "NZDJPY");
		intToString.put(6, "SGDJPY");
		intToString.put(7, "USDJPY");
		intToString.put(8, "ZARJPY");
		intToString.put(9, "AUDCHF");
		intToString.put(10, "AUDUSD");
		intToString.put(11, "EURCHF");
		intToString.put(12, "GBPAUD");
		intToString.put(13, "GBPNZD");
		intToString.put(14, "TRYJPY");
		intToString.put(15, "USDSGD");
		intToString.put(16, "CADCHF");
		intToString.put(17, "CHFJPY");
		intToString.put(18, "EURJPY");
		intToString.put(19, "EURSGD");
		intToString.put(20, "GBPCAD");
		intToString.put(21, "NZDCAD");
		intToString.put(22, "NZDUSD");
		intToString.put(23, "USDCAD");
		intToString.put(24, "USDHKD");
		intToString.put(25, "AUDJPY");
		intToString.put(26, "EURTRY");
		intToString.put(27, "GBPCHF");
		intToString.put(28, "GBPSGD");
		intToString.put(29, "NZDCHF");
		intToString.put(30, "USDCHF");
		intToString.put(31, "USDTRY");
		intToString.put(32, "AUDNZD");
		intToString.put(33, "CADJPY");
		intToString.put(34, "EURAUD");
		intToString.put(35, "EURGBP");
		intToString.put(36, "EURNZD");
		intToString.put(37, "EURUSD");
		intToString.put(38, "GBPUSD");
		intToString.put(39, "USDCNH");
		intToString.put(40, "USDINR");
	}

	public static Map<Integer, ConcurrentLinkedQueue<InstantSignal>> getINSTRUMENT_TO_QUEUE() {
		return INSTRUMENT_TO_QUEUE;
	}

	public static Map<String, Integer> getStringToInt() {
		return stringToInt;
	}

	public static Map<Integer, String> getIntToString() {
		return intToString;
	}

}
