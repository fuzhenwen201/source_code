/**
 * 
 */
package com.broctagon.tradstnode.rabbit.utils;

import java.sql.Time;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

/**
 * Utility class for Date related operations
 * 
 * @author Kanhaiya
 *
 */
public class DateUtils {

	public static final String DdMmYyyyHmsFormatWithZone = "dd:MM:yyyy hh:mm:ss a z";
	public static final String TIME_FORMAT = "hh:mm:ss a z";
	
	/**
	 * Return Current Date day intValue
	 */
	public static int getCurrentDate(){
		Date date = new Date(System.currentTimeMillis());
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		int day = calendar.get(Calendar.DATE);
		return day;
	}
	
	/**
	 * Return string representation of current date in dd_MM_yyyy_hour_minute_second format
	 * @param date
	 */
	public static String getDateTimeStringFormat(){
		Date date = new Date(System.currentTimeMillis());
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		int year = calendar.get(Calendar.YEAR);
		int month = calendar.get(Calendar.MONTH) + 1;
		int day = calendar.get(Calendar.DATE);
		int hour = calendar.get(Calendar.HOUR_OF_DAY);
		int minute = calendar.get(Calendar.MINUTE);
		int second = calendar.get(Calendar.SECOND);
		return new StringBuilder().append(day+"_").append(month+"_").append(year+"_").append(hour+"_").append(minute+"_").append(second).toString();
	}
	
	/**
	 * Return converted String date value by given date and format
	 */
	public static String convertDateToString(Date date, String format) {
		SimpleDateFormat formatter = new SimpleDateFormat(format);
		return formatter.format(date);
	}
	
	/**
	 * Return java Util Date value by given String date and format
	 */
	public static Date convertStringToDate(String date, String format) throws ParseException {
		SimpleDateFormat formatter = new SimpleDateFormat(format);
		return formatter.parse(date);
	}
	
	public static Time convertStringtoTime(String str) throws ParseException
	{
		DateFormat originalFormat = new SimpleDateFormat("HH:MM:SS");
		DateFormat targetFormat = new SimpleDateFormat("HH:MM");
		Date date = originalFormat.parse(str);
		String formattedDate = targetFormat.format(date);
		java.sql.Time timeValue = new java.sql.Time(targetFormat.parse(formattedDate).getTime());
		return timeValue;
	}
	
	public static void main(String[] args) {
		//System.out.println("date = " + convertDateToString(new Date(), DdMmYyyyHmsFormatWithZone));
		//System.out.println("date = " + convertDateToString(new Date(), TIME_FORMAT));
		//System.out.println("getDateTimeStringFormat() = "+ getDateTimeStringFormat());
		
		// Hex to Integer convertor
		Integer intValue = Integer.parseInt("0083", 16);
		Short shortValue = Short.parseShort("0084", 16);
		System.out.println("intValue = "+intValue +" shortValue = "+ shortValue );
		
		// Hex to Short Converter
		short shortV1 = (short) 0x22;
		short shortV2 = (short) 0x24;
		System.out.println("shortV1 = " + shortV1 +" shortV2 = " + shortV2);
		
	}
}
