
package cn.fintechstar.dc.util;

import java.sql.Timestamp;
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.YearMonth;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;

import cn.fintechstar.dc.constant.GranularityConstant;

public class DateUtil {
	
	static DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");

	/**
	 * convert timestamp to the dateTime
	 * 1501740893000262 -> 2017-08-03T14:14:53
	 */
	public static LocalDateTime getCurrentDateTime(long timestamp){
		Instant instant = Instant.ofEpochMilli(timestamp/1000);
		LocalDateTime dateTime = LocalDateTime.ofInstant(instant, ZoneId.systemDefault());
		return dateTime;
	}

	public static String getHistoricalSpreadTime(String granularity, LocalDateTime requestTime, int i) {
		long seconds = DateUtil.getSecondsByGranularity(granularity, Timestamp.valueOf(requestTime).getTime());
		LocalDateTime dateTime = requestTime.minusSeconds(seconds * i);
		return dateTime.toString();
	}
	
	public static long getSecondsByGranularity(String granularity, long timestamp){
		if("M".equals(granularity)){
			LocalDateTime dateTime = getCurrentDateTime(timestamp);
			YearMonth yearMonth = YearMonth.from(dateTime.minusMonths(1));
			int days = yearMonth.lengthOfMonth();
			return days * 24 * 60 * 60;
		}else {
			return GranularityConstant.getSecondsByGranularity(granularity);
		}
	}
}
