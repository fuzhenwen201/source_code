package cn.fintechstar.admin.util;

import java.sql.Time;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Date;

import org.springframework.util.StringUtils;

public class DateUtil {

	
	private static DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
	
	public static String getCurrentDateTimeStr(){
		LocalDateTime dateTime = LocalDateTime.now();
		return dateTimeFormatter.format(dateTime);
	}
	
	public static Time convertStringtoTime(String str) throws ParseException{
		if(!StringUtils.isEmpty(str)){
			DateFormat originalFormat = new SimpleDateFormat("HH:MM:SS");
			DateFormat targetFormat = new SimpleDateFormat("HH:MM");
			Date date = originalFormat.parse(str);
			String formattedDate = targetFormat.format(date);
			java.sql.Time timeValue = new java.sql.Time(targetFormat.parse(formattedDate).getTime());
			return timeValue;
		}else{
			return null;
		}

	}
}
