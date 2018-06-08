package com.broctagon.accumonitor.util;

public class ConstantUtil {
	
	public static String getMessageFromRequest(String message, String keyWord){
		String[] messages = message.split("\"" + keyWord + "\":");
		messages = messages[1].split(",");
		return messages[0].replaceAll("\"", "");
	}
	
	public static Integer getSgId(String message){
		String[] messages = message.split("\"sgid\":");
		messages = messages[1].split(",");
		return Integer.parseInt(messages[0].replaceAll("\"", ""));
	}
	
	public static String getRoutingKey(String message){
		return message.replace("#", "$");
	}
	
}

