package cn.fintechstar.traddc.uitl;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

import org.junit.Assert;
import org.junit.Test;

import cn.fintechstar.dcb.util.DateUtil;

public class DateUtilTest {

	DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
	
	@Test
	public void getCurrentDateTimeTest(){
		long timestamp = 1501740893000262L;
		Assert.assertEquals("2017-08-03 14:14:53", DateUtil.getCurrentDateTimeStr(timestamp));
	}
	
	@Test
	public void getMarketStartTimeBySecondTest(){
		LocalDateTime dateTime = DateUtil.getCurrentDateTime(1501740893000262L);
		LocalDateTime d1 = DateUtil.getMarketStartTimeBySecond(dateTime, 5);
		Assert.assertEquals("2017-08-03T14:14:50", d1.toString());
		LocalDateTime d2 = DateUtil.getMarketStartTimeBySecond(dateTime, 10);
		Assert.assertEquals("2017-08-03T14:14:50", d2.toString());
		LocalDateTime d3 = DateUtil.getMarketStartTimeBySecond(dateTime, 15);
		Assert.assertEquals("2017-08-03T14:14:45", d3.toString());
		LocalDateTime d4 = DateUtil.getMarketStartTimeBySecond(dateTime, 30);
		Assert.assertEquals("2017-08-03T14:14:30", d4.toString());
	}
	
	@Test
	public void getMarketEndTimeBySecondTest(){
		LocalDateTime dateTime = DateUtil.getCurrentDateTime(1501740893000262L);
		LocalDateTime d1 = DateUtil.getMarketEndTimeBySecond(dateTime, 5);
		Assert.assertEquals("2017-08-03 14:14:55", dateTimeFormatter.format(d1));
		LocalDateTime d2 = DateUtil.getMarketEndTimeBySecond(dateTime, 10);
		Assert.assertEquals("2017-08-03 14:15:00", dateTimeFormatter.format(d2));
		LocalDateTime d3 = DateUtil.getMarketEndTimeBySecond(dateTime, 15);
		Assert.assertEquals("2017-08-03 14:15:00", dateTimeFormatter.format(d3));
		LocalDateTime d4 = DateUtil.getMarketEndTimeBySecond(dateTime, 30);
		Assert.assertEquals("2017-08-03 14:15:00", dateTimeFormatter.format(d4));
	}
	
	@Test
	public void getMarketStartTimeByMinuteTest(){
		LocalDateTime dateTime = DateUtil.getCurrentDateTime(1501740893000262L);
		LocalDateTime d1 = DateUtil.getMarketStartTimeByMinute(dateTime, 1);
		Assert.assertEquals("2017-08-03 14:14:00", dateTimeFormatter.format(d1));
		LocalDateTime d2 = DateUtil.getMarketStartTimeByMinute(dateTime, 5);
		Assert.assertEquals("2017-08-03 14:10:00", dateTimeFormatter.format(d2));
		LocalDateTime d3 = DateUtil.getMarketStartTimeByMinute(dateTime, 10);
		Assert.assertEquals("2017-08-03 14:10:00", dateTimeFormatter.format(d3));
		LocalDateTime d4 = DateUtil.getMarketStartTimeByMinute(dateTime, 15);
		Assert.assertEquals("2017-08-03 14:00:00", dateTimeFormatter.format(d4));
		LocalDateTime d5 = DateUtil.getMarketStartTimeByMinute(dateTime, 30);
		Assert.assertEquals("2017-08-03 14:00:00", dateTimeFormatter.format(d5));
	}
	
	@Test 
	public void getMarketEndTimeByMinuteTest(){
		LocalDateTime dateTime = DateUtil.getCurrentDateTime(1501740893000262L);
		LocalDateTime d1 = DateUtil.getMarketEndTimeByMinute(dateTime, 1);
		Assert.assertEquals("2017-08-03 14:15:00", dateTimeFormatter.format(d1));
		LocalDateTime d2 = DateUtil.getMarketEndTimeByMinute(dateTime, 5);
		Assert.assertEquals("2017-08-03 14:15:00", dateTimeFormatter.format(d2));
		LocalDateTime d3 = DateUtil.getMarketEndTimeByMinute(dateTime, 10);
		Assert.assertEquals("2017-08-03 14:20:00", dateTimeFormatter.format(d3));
		LocalDateTime d4 = DateUtil.getMarketEndTimeByMinute(dateTime, 15);
		Assert.assertEquals("2017-08-03 14:15:00", dateTimeFormatter.format(d4));
		LocalDateTime d5 = DateUtil.getMarketEndTimeByMinute(dateTime, 30);
		Assert.assertEquals("2017-08-03 14:30:00", dateTimeFormatter.format(d5));
	}
	
	@Test
	public void getMarketStartTimeByHourTest(){
		LocalDateTime dateTime = DateUtil.getCurrentDateTime(1501740893000262L);
		LocalDateTime d1 = DateUtil.getMarketStartTimeByHour(dateTime, 1);
		Assert.assertEquals("2017-08-03 14:00:00", dateTimeFormatter.format(d1));
		LocalDateTime d2 = DateUtil.getMarketStartTimeByHour(dateTime, 4);
		Assert.assertEquals("2017-08-03 12:00:00", dateTimeFormatter.format(d2));
		LocalDateTime d3 = DateUtil.getMarketStartTimeByHour(dateTime, 12);
		Assert.assertEquals("2017-08-03 12:00:00", dateTimeFormatter.format(d3));
	}
	
	@Test
	public void getMarketEndTimeByHourTest(){
		LocalDateTime dateTime = DateUtil.getCurrentDateTime(1501740893000262L);
		LocalDateTime d1 = DateUtil.getMarketEndTimeByHour(dateTime, 1);
		Assert.assertEquals("2017-08-03 15:00:00", dateTimeFormatter.format(d1));
		LocalDateTime d2 = DateUtil.getMarketEndTimeByHour(dateTime, 4);
		Assert.assertEquals("2017-08-03 16:00:00", dateTimeFormatter.format(d2));
		LocalDateTime d3 = DateUtil.getMarketEndTimeByHour(dateTime, 12);
		Assert.assertEquals("2017-08-04 00:00:00", dateTimeFormatter.format(d3));
	}
	
	@Test
	public void getTimestampTest(){
		long timestamp = DateUtil.getTimestamp("2017-08-03 14:14:53");
		Assert.assertEquals(1501740893000000L, timestamp);
	}
	
	@Test
	public void getDValueBetweenTwoTimestampTest(){
		int dValue = DateUtil.getDValueBetweenTwoTimestamp(1501740893000262L, 1501740902820262L, "S5");
		Assert.assertEquals(1, dValue);
		dValue = DateUtil.getDValueBetweenTwoTimestamp(1501740889500262L, 1501740999880262L, "S5");
		Assert.assertEquals(21, dValue);
		dValue = DateUtil.getDValueBetweenTwoTimestamp(1501740889500262L, 1501740999880262L, "M1");
		Assert.assertEquals(1, dValue);
	}
	
	@Test
	public void getPreMarketEndTimeTest(){
		//14:14:53
		String str = DateUtil.getPreMarketEndTime(1501740893000262L, 2, "S5");
		Assert.assertEquals("2017-08-03 14:14:40", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "S10");
		Assert.assertEquals("2017-08-03 14:14:20", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "S15");
		Assert.assertEquals("2017-08-03 14:14:00", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "S30");
		Assert.assertEquals("2017-08-03 14:13:00", str);		
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "M1");
		Assert.assertEquals("2017-08-03 14:11:00", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "M5");
		Assert.assertEquals("2017-08-03 13:55:00", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "M10");
		Assert.assertEquals("2017-08-03 13:40:00", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "M15");
		Assert.assertEquals("2017-08-03 13:15:00", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "M30");
		Assert.assertEquals("2017-08-03 12:30:00", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "H1");
		Assert.assertEquals("2017-08-03 11:00:00", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "H4");
		Assert.assertEquals("2017-08-03 00:00:00", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "H12");
		Assert.assertEquals("2017-08-02 00:00:00", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "D");
		Assert.assertEquals("2017-07-30 00:00:00", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "W");
		Assert.assertEquals("2017-07-10 00:00:00", str);
		str = DateUtil.getPreMarketEndTime(1501740893000262L, 3, "M");
		Assert.assertEquals("2017-05-01 00:00:00", str);
	}
}
