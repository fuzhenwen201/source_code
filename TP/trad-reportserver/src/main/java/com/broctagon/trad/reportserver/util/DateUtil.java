package com.broctagon.trad.reportserver.util;

import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class DateUtil {
	
	private static final String DATE_FORMAT = "yyyy/MM/dd HH:mm:ss";

	private static DateFormat sdf = new SimpleDateFormat(DATE_FORMAT);
	
	
	public static String getString(Date date){
		return sdf.format(date);
	}
	
	public static Timestamp getTimestamp(Date date){
		return new Timestamp(date.getTime());
	}
	
	public static Date getDate(Timestamp timestamp){
		return timestamp;
	}
}
