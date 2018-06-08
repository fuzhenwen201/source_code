package com.broctagon.tradstnode.rabbit.utils;

import java.lang.reflect.Type;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonParseException;

/**
 * Utility class for JsonParser operations
 * 
 * @author Kanhaiya
 *
 */
public class JsonParserUtil {
	
	/**
	 * Convert JsonString to Json Object of Type T by passing JsonStr and classOfT
	 * @param json
	 * @param classOfT
	 */
	public static <T> T getObjectByJsonStr(String json, Class<T> classOfT) throws JsonParseException {
		Gson gson = new GsonBuilder().create();
	    return gson.fromJson(json.trim(), (Type) classOfT);
	}
	
	/**
	 * Convert Object to Json String
	 * @param src
	 * @return
	 */
	public static String toJsonString(Object src) {
		Gson gson = new GsonBuilder().create();
	    return gson.toJson(src);
	}
}
