package cn.fintechstar.traddc.constant;

import java.util.HashMap;
import java.util.Map;

import cn.fintechstar.traddc.sbe.BooleanType;

public class SbeMap {
	
	private static Map<Boolean, BooleanType> sbBooleanMap = new HashMap<>();
	
	private static Map<BooleanType, Boolean> dcBooleanMap = new HashMap<>();
	
	static{
		sbBooleanMap.put(true, BooleanType.T);
		sbBooleanMap.put(false, BooleanType.F);
		
		dcBooleanMap.put(BooleanType.T, true);
		dcBooleanMap.put(BooleanType.F, false);
	}

	public static BooleanType getSbeBooleanType(Boolean booleanType){
		return sbBooleanMap.get(booleanType);
	}
	
	public static Boolean getDcBooleanType(BooleanType booleanType){
		return dcBooleanMap.get(booleanType);
	}

}
