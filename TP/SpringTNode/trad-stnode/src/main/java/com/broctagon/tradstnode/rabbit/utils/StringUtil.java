/**
 * 
 */
package com.broctagon.tradstnode.rabbit.utils;

import java.util.Date;
import java.util.List;
import java.util.regex.Pattern;

/**
 * Utility class for String related operations
 * 
 * @author Kanhaiya
 * 
 */
public class StringUtil {

	/**
	 * Checks whether a String value is empty or null
	 * 
	 * @param value
	 */
	public static boolean isNull(Object value) {
		String strValue = null;
		if (value instanceof Integer || value instanceof Long || value instanceof String || value instanceof Date) {
			strValue = value.toString();
		}
		if ((strValue == null) || (strValue != null
				&& ("".equals(strValue.trim()) || "null".equals(strValue.trim()) || strValue.trim().length() == 0))) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Checks whether a String value is empty or null
	 * 
	 * @param value
	 */
	public static boolean equals(Object object1, Object object2) {
		if (object1 == null || object2 == null) {
			return Boolean.FALSE;
		}
		return object1.equals(object2);
	}

	/**
	 * Checks whether a String value is empty or null
	 * 
	 * @param value
	 */
	public static boolean isNotNull(Object value) {
		return !isNull(value);
	}

	/**
	 * Convert String array List into separated comma string
	 * 
	 * @param strList
	 */
	public static String seperatedCommaString(List<String> strList) {
		StringBuilder strBuilder = new StringBuilder();
		for (int i = 0; i < strList.size(); i++) {
			if (i == strList.size() - 1) {
				strBuilder.append(strList.get(i));
			} else {
				strBuilder.append(strList.get(i) + ", ");
			}
		}
		return strBuilder.toString();
	}

	/**
	 * This method splits any comma, space and arrow separated string.
	 * 
	 * @param str
	 * @return array of string
	 */
	public static String[] split(String str) {
		final String REGEX = "\\s*(\\s|=>|,)\\s*";
		final Pattern p = Pattern.compile(REGEX);
		return p.split(str);
	}

	/**
	 * Split sting with commas
	 * 
	 * @param str
	 * @return
	 */
	public static String[] splitWithComma(String str) {
		final String REGEX = ",\\s*";
		final Pattern p = Pattern.compile(REGEX);
		return p.split(str);
	}

	/**
	 * This method is being used to remove special character from string which
	 * &, /, %, '. We might need this later
	 * 
	 * @param data
	 */
	public static String stripData(String data) {
		// Guard Clause
		if (isNull(data))
			return data;
		data = data.replaceAll("&", " and ");
		data = data.replaceAll("/", " ");
		data = data.replaceAll("%", " ");
		data = data.replaceAll("'", "");
		return data;
	}

	/**
	 * Cleans Up the string and keep meaningful characters
	 * 
	 * @param data
	 */
	public static String cleanData(String data) {
		data = data.replaceAll("&", " and ");
		data = data.replaceAll("/", " ");
		data = data.replaceAll("%", " ");
		data = data.replaceAll("'", "");
		return data;
	}

}
