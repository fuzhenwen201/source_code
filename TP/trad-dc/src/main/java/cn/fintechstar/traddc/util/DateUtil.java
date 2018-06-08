
package cn.fintechstar.traddc.util;

import java.sql.Timestamp;
import java.time.DayOfWeek;
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.YearMonth;
import java.time.Year;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.time.temporal.ChronoUnit;
import java.time.temporal.TemporalAdjusters;

import cn.fintechstar.traddc.constant.GranularityConstant;
import cn.fintechstar.traddc.model.Granularity;

public class DateUtil {
	
	static DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
	
	/**
	 * convert timestamp to the String
	 * 1501740893000262 -> 2017-08-03T14:14:53
	 */
	public static String getCurrentDateTimeStr(long timestamp){
		LocalDateTime dateTime = getCurrentDateTime(timestamp);
		return dateTimeFormatter.format(dateTime);
	}
	
	/**
	 * convert timestamp to the dateTime
	 * 1501740893000262 -> 2017-08-03T14:14:53
	 */
	public static LocalDateTime getCurrentDateTime(long timestamp){
		Instant instant = Instant.ofEpochMilli(timestamp/1000);
		LocalDateTime dateTime = LocalDateTime.ofInstant(instant, ZoneId.systemDefault());
		return dateTime;
	}
	
	/**
	 *  convert dateTime to timestamp
	 *  2017-08-03T14:14:53 -> 1501740893000000
	 */
	public static long getTimestamp(String dateTimeStr){
		LocalDateTime dateTime = LocalDateTime.parse(dateTimeStr, dateTimeFormatter);
		return Timestamp.valueOf(dateTime).getTime() * 1000;
	}

	/**
	 * Get the marketEndTime of HistoricalCandle by granularity according to the timestamp
	 * e.g:
	 *	input : 1501740893000262L(2017-08-03 14:14:53)
	 *  output : 
	 *  	S5  : 2017-08-03 14:14:55
	 *  	S10 : 2017-08-03 14:15:00
	 *  	S15 : 2017-08-03 14:15:00
	 *  	S30 : 2017-08-03 14:15:00
	 *  	
	 *  	M1  : 2017-08-03 14:15:00
	 *  	M5  : 2017-08-03 14:15:00
	 *  	M10 : 2017-08-03 14:20:00
	 *  	M15 : 2017-08-03 14:15:00
	 *  	M30 : 2017-08-03 14:30:00  
	 *  
	 *  	H1  : 2017-08-03 15:00:00
	 *  	H4  : 2017-08-03 16:00:00
	 *  	H12 : 2017-08-04 00:00:00
	 *  
	 *  	D   : 2017-08-04 00:00:00
	 *  	W   : 2017-08-07 00:00:00
	 *  	M   : 2017-09-01 00:00:00
	 */
	public static String getMarketEndTimeStrByGranularity(long timestamp, Granularity granularity) {
		LocalDateTime dateTime = getMarketEndTimeByGranularity(timestamp, granularity);
		return dateTimeFormatter.format(dateTime);
	}
	
	public static LocalDateTime getMarketEndTimeByGranularity(long timestamp, Granularity granularity){
		LocalDateTime dateTime = getCurrentDateTime(timestamp);
		if(GranularityConstant.getSecondsMap().containsKey(granularity)){
			return getMarketEndTimeBySecond(dateTime, GranularityConstant.getSecondsMap().get(granularity));
		}else if(GranularityConstant.getMinutesMap().containsKey(granularity)){
			return getMarketEndTimeByMinute(dateTime, GranularityConstant.getMinutesMap().get(granularity));
		}else if(GranularityConstant.getHoursMap().containsKey(granularity)){
			return getMarketEndTimeByHour(dateTime, GranularityConstant.getHoursMap().get(granularity));
		}else if(Granularity.D.equals(granularity)){
			return dateTime.plusDays(1).withHour(0).withMinute(0).withSecond(0);
		}else if(Granularity.W.equals(granularity)){
			return dateTime.with(TemporalAdjusters.nextOrSame(DayOfWeek.MONDAY)).withHour(0).withMinute(0).withSecond(0);
		}else if(Granularity.M.equals(granularity)){
			return dateTime.plusMonths(1).withDayOfMonth(1).withHour(0).withMinute(0).withSecond(0);
		}
		return dateTime;
	}
	
	/**
	 * Get the marketStartTime of HistoricalCandle by granularity according to the timestamp
	 * e.g:
	 *	input : 1501740893000262L(2017-08-03 14:14:53)
	 *  output : 
	 *  	S5  : 2017-08-03 14:14:50
	 *  	S10 : 2017-08-03 14:14:50
	 *  	S15 : 2017-08-03 14:14:45
	 *  	S30 : 2017-08-03 14:14:30
	 *  	
	 *  	M1  : 2017-08-03 14:14:00
	 *  	M5  : 2017-08-03 14:10:00
	 *  	M10 : 2017-08-03 14:10:00
	 *  	M15 : 2017-08-03 14:00:00
	 *  	M30 : 2017-08-03 14:00:00  
	 *  
	 *  	H1  : 2017-08-03 14:00:00
	 *  	H4  : 2017-08-03 12:00:00
	 *  	H12 : 2017-08-03 12:00:00
	 *  
	 *  	D   : 2017-08-03 00:00:00
	 *  	W   : 2017-07-31 00:00:00
	 *  	M   : 2017-08-01 00:00:00
	 */
	public static String getMarketStartTimeStrByGranularity(long timestamp, Granularity granularity) {
		LocalDateTime dateTime = getMarketStartTimeByGranularity(timestamp, granularity);
		return dateTimeFormatter.format(dateTime);
	}

	public static LocalDateTime getMarketStartTimeByGranularity(long timestamp, Granularity granularity) {
		LocalDateTime dateTime = getCurrentDateTime(timestamp);
		if(GranularityConstant.getSecondsMap().containsKey(granularity)){
			return getMarketStartTimeBySecond(dateTime, GranularityConstant.getSecondsMap().get(granularity));
		}else if(GranularityConstant.getMinutesMap().containsKey(granularity)){
			return getMarketStartTimeByMinute(dateTime, GranularityConstant.getMinutesMap().get(granularity));
		}else if(GranularityConstant.getHoursMap().containsKey(granularity)){
			return getMarketStartTimeByHour(dateTime, GranularityConstant.getHoursMap().get(granularity));
		}else if(Granularity.D.equals(granularity)){
			return dateTime.minusDays(1).withHour(0).withMinute(0).withSecond(0);
		}else if(Granularity.W.equals(granularity)){
			return dateTime.with(TemporalAdjusters.previousOrSame(DayOfWeek.MONDAY)).withHour(0).withMinute(0).withSecond(0);
		}else if(Granularity.M.equals(granularity)){
			return dateTime.withDayOfMonth(1).withHour(0).withMinute(0).withSecond(0);
		}
		return dateTime;
	}
	
	/**
	 * Get the marketEndTime of HistoricalCandle by second
	 * e.g:
	 *	input : 1501740893000262(2017-08-03 14:14:53)
	 *  output : 
	 *  	S5  : 2017-08-03 14:14:55
	 *  	S10 : 2017-08-03 14:15:00
	 *  	S15 : 2017-08-03 14:15:00
	 *  	S30 : 2017-08-03 14:15:00
	 */
	public static LocalDateTime getMarketEndTimeBySecond(LocalDateTime dateTime, int duration) {
		LocalDateTime marketStartTime = getMarketStartTimeBySecond(dateTime, duration);
		LocalDateTime marketEndTime = marketStartTime.plusSeconds(duration);
		return marketEndTime;
	}
	
	
	/**
	 * Get the marketStartTime of HistoricalCandle by minute
	 * e.g:
	 *	input : 1501740893000262(2017-08-03 14:14:53)
	 *  output : 
	 *  	S5  : 2017-08-03 14:14:55
	 *  	S10 : 2017-08-03 14:15:00
	 *  	S15 : 2017-08-03 14:15:00
	 *  	S30 : 2017-08-03 14:15:00
	 */
	public static LocalDateTime getMarketStartTimeBySecond(LocalDateTime dateTime, int duration){
		int second = dateTime.getSecond() / duration * duration;
		LocalDateTime marketStartTime = dateTime.withSecond(second);
		return marketStartTime;
	}
	
	/**
	 * Get the marketEndTime of HistoricalCandle by granularity according to the timestamp
	 * e.g:
	 *	input : 1501740893000262(2017-08-03 14:14:53)
	 *  output : 
	 *  	M1  : 2017-08-03 14:15:00
	 *  	M5  : 2017-08-03 14:15:00
	 *  	M10 : 2017-08-03 14:20:00
	 *  	M15 : 2017-08-03 14:15:00
	 *  	M30 : 2017-08-03 14:30:00  
	 */
	public static LocalDateTime getMarketEndTimeByMinute(LocalDateTime dateTime, Integer duration) {
		LocalDateTime marketStartTime = getMarketStartTimeByMinute(dateTime, duration);
		LocalDateTime marketEndTime = marketStartTime.plusMinutes(duration);
		return marketEndTime;
	}

	/**
	 * Get the marketStartTime of HistoricalCandle by minute
	 * e.g:
	 *	input : 1501740893000262(2017-08-03 14:14:53)
	 *  output : 
	 *  	M1  : 2017-08-03 14:14:00
	 *  	M5  : 2017-08-03 14:10:00
	 *  	M10 : 2017-08-03 14:10:00
	 *  	M15 : 2017-08-03 14:00:00
	 *  	M30 : 2017-08-03 14:00:00  
	 */
	public static LocalDateTime getMarketStartTimeByMinute(LocalDateTime dateTime, Integer duration) {
		int minute = dateTime.getMinute() / duration * duration;
		LocalDateTime marketStartTime = dateTime.withMinute(minute).withSecond(0);
		return marketStartTime;
	}

	/**
	 * Get the marketEndTime of HistoricalCandle by hour
	 * e.g:
	 *	input : 1501740893000262(2017-08-03 14:14:53)
	 *  output : 
	 *  	H1  : 2017-08-03 15:00:00
	 *  	H4  : 2017-08-03 16:00:00
	 *  	H12 : 2017-08-04 00:00:00
	 */
	public static LocalDateTime getMarketEndTimeByHour(LocalDateTime dateTime, Integer duration) {
		LocalDateTime marketStartTime = getMarketStartTimeByHour(dateTime, duration);
		LocalDateTime marketEndTime = marketStartTime.plusHours(duration);
		return marketEndTime;
	}

	/**
	 * Get the marketStartTime of HistoricalCandle by hour
	 * e.g:
	 *	input : 1501740893000262(2017-08-03 14:14:53)
	 *  output : 
	 *  	H1  : 2017-08-03 14:00:00
	 *  	H4  : 2017-08-03 12:00:00
	 *  	H12 : 2017-08-04 12:00:00
	 */
	public static LocalDateTime getMarketStartTimeByHour(LocalDateTime dateTime, Integer duration) {
		int hour = dateTime.getHour() / duration * duration;
		LocalDateTime marketStartTime = dateTime.withHour(hour).withMinute(0).withSecond(0);
		return marketStartTime;
	}

	/**
	 * calculate duration according to market time
	 * e.g: 
	 *   input : 1501740893000262L(2017-08-03 14:14:53) -> 2017-08-03 14:14:55
	 * 	         1501740902820262L(2017-08-03 14:15:02) -> 2017-08-03 14:15:00
	 *           Granularity.S5
	 *   output : 1(2017-08-03 14:15:00)
	 * 
 	 *   input : 1501740889500262L(2017-08-03 14:14:49) -> 2017-08-03 14:14:50
	 * 		     1501740999880262L(2017-08-03 14:16:39) -> 2017-08-03 14:16:35
	 *           Granularity.S5
	 *   output : 21
	 *   (2017-08-03 14:14:55, 2017-08-03 14:15:00, 2017-08-03 14:15:05, 2017-08-03 14:15:10, 2017-08-03 14:15:15, 
	 *    2017-08-03 14:15:20, 2017-08-03 14:15:25, 2017-08-03 14:15:30, 2017-08-03 14:15:35, 2017-08-03 14:15:40,
	 *    2017-08-03 14:15:45, 2017-08-03 14:15:50, 2017-08-03 14:15:55, 2017-08-03 14:16:00, 2017-08-03 14:16:05,
	 *    2017-08-03 14:16:10, 2017-08-03 14:16:15, 2017-08-03 14:16:20, 2017-08-03 14:16:25, 2017-08-03 14:16:30,
	 *    2017-08-03 14:16:35)
	 * @param granularity 
	 */
	public static int getDValueBetweenTwoTimestamp(long previousTime, long currentTime, Granularity granularity) {
		LocalDateTime previousMarketEndTime = getMarketEndTimeByGranularity(previousTime, granularity);
		LocalDateTime currentMarketStartTime = getMarketStartTimeByGranularity(currentTime, granularity);
		long previousMarketEndTimeInMillis = Timestamp.valueOf(previousMarketEndTime).getTime();
		long currentMarketStartTimeInMills = Timestamp.valueOf(currentMarketStartTime).getTime();
		int dValue = (int) ((currentMarketStartTimeInMills - previousMarketEndTimeInMillis) / 1000 / DateUtil.getSecondsByGranularity(granularity, currentMarketStartTimeInMills));
		return dValue;
	}
	
	public static String getPreMarketEndTime(long signalTime, int count, Granularity granularity){
		LocalDateTime currentDateTime = getMarketStartTimeByGranularity(signalTime, granularity);
		if(GranularityConstant.getSecondsMap().containsKey(granularity)){
			LocalDateTime marketEndTime = currentDateTime.minusSeconds(GranularityConstant.getSecondsMap().get(granularity) * count);
			return dateTimeFormatter.format(marketEndTime);
		}else if(GranularityConstant.getMinutesMap().containsKey(granularity)){
			LocalDateTime marketEndTime = currentDateTime.minusMinutes(GranularityConstant.getMinutesMap().get(granularity) * count).withSecond(0);
			return dateTimeFormatter.format(marketEndTime);
		}else if(GranularityConstant.getHoursMap().containsKey(granularity)){
			LocalDateTime marketEndTime = currentDateTime.minusHours(GranularityConstant.getHoursMap().get(granularity) * count).withMinute(0).withSecond(0);
			return dateTimeFormatter.format(marketEndTime);
		}else if(Granularity.D.equals(granularity)){
			LocalDateTime marketEndTime = currentDateTime.minusDays(count).withHour(0).withMinute(0).withSecond(0);
			return dateTimeFormatter.format(marketEndTime);
		}else if(Granularity.W.equals(granularity)){
			LocalDateTime marketEndTime = currentDateTime.minusDays(7 * count).withHour(0).withMinute(0).withSecond(0);
			return dateTimeFormatter.format(marketEndTime);
		}else if(Granularity.M.equals(granularity)){
			LocalDateTime marketEndTime = currentDateTime.minusMonths(count).withDayOfMonth(1).withHour(0).withMinute(0).withSecond(0);
			return dateTimeFormatter.format(marketEndTime);
		}else {
			return dateTimeFormatter.format(currentDateTime);
		}
	}

	public static String getHistoricalSpreadTime(Granularity granularity, LocalDateTime requestTime, int i) {
		long seconds = DateUtil.getSecondsByGranularity(granularity, Timestamp.valueOf(requestTime).getTime());
		LocalDateTime dateTime = requestTime.minusSeconds(seconds * i);
		return dateTime.toString();
	}
	
	public static long getSecondsByGranularity(Granularity granularity, long timestamp){
		if(Granularity.M.equals(granularity)){
			LocalDateTime dateTime = getCurrentDateTime(timestamp);
			YearMonth yearMonth = YearMonth.from(dateTime.minusMonths(1));
			int days = yearMonth.lengthOfMonth();
			return days * 24 * 60 * 60;
		}else {
			return GranularityConstant.getSecondsByGranularity(granularity);
		}
	}
}
