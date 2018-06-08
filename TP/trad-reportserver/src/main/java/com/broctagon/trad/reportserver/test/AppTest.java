package com.broctagon.trad.reportserver.test;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.reportserver.common.CommonConstants;
import com.broctagon.trad.reportserver.config.DruidConfig;
import com.broctagon.trad.reportserver.config.SpringConfig;
import com.broctagon.trad.reportserver.dao.PriceDao;
import com.broctagon.trad.reportserver.dao.TradeDao;
import com.broctagon.trad.reportserver.model.Price;
import com.broctagon.trad.reportserver.model.Trade;
import com.broctagon.trad.reportserver.service.MT4ReportService;
import com.broctagon.trad.reportserver.util.DateUtil;

public class AppTest {

    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        
        ApplicationContext appContext = new AnnotationConfigApplicationContext(SpringConfig.class);
        
        TradeDao tradeDao = appContext.getBean(TradeDao.class);
        int ticket = tradeDao.getTicketByOrderId("294eff927b9111e7aaf737c962a51b48");
        Trade trade = tradeDao.getTradeByOrderId("294eff927b9111e7aaf737c962a51b48");
        System.out.println("ticket:"+ticket);
        System.out.println("trade:"+JSON.toJSONString(trade));
        
/*        long currentTimeMillis = System.currentTimeMillis();
        System.out.println("System.currentTimeMillis():"+currentTimeMillis);
        Date date = new Date();
        System.out.println("date.getTime():"+date.getTime());
        Timestamp ts = new Timestamp(currentTimeMillis);
        System.out.println("ts:" + ts.toString());*/
        
  //      Timestamp ts = DateUtil.getTimestamp(new Date());
  //      System.out.println(System.currentTimeMillis());
     	
 /*    	String city1 = "2343iu48397ioewukljkdslkjdskljm";
     	System.out.println(city1.length());
     	System.out.println(city1.substring(0, 30));*/
        
/*        UserDao userDao = appContext.getBean(UserDao.class);
        User user = userDao.getUserByLoginId(32782);
        System.out.println("user:" + JSON.toJSONString(user));*/
        
/*        PriceDao priceDao = appContext.getBean(PriceDao.class);
        testAddPrice(priceDao);
        Price price = priceDao.getPriceBySymbol("AFRICASTATE");
        System.out.println("price:" + JSON.toJSONString(price));*/
        
/*        UserDaoTestWater userDaoTest = appContext.getBean(UserDaoTestWater.class);
        try {
			userDaoTest.addUsers();
		} catch (SerialException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}*/
    }
    
	public static void testAddPrice(PriceDao priceDao){
		
		List<Price> prices = new ArrayList<Price>();
		
		Price price = new Price();
		price.setSymbol("AFRICASTATE");
		price.setTime(DateUtil.getTimestamp(new Date()));
		price.setBid(3.3);
		price.setAsk(1.5);
		price.setLow(1.3);
		price.setHigh(1.8);
		price.setDirection(5);
		price.setDigits(8);
		price.setSpread(6);
		price.setModify_time(DateUtil.getTimestamp(new Date()));	
		prices.add(price);
		
		price = new Price();
		price.setSymbol("AFRICASTATEs");
		price.setTime(DateUtil.getTimestamp(new Date()));
		price.setBid(3.4);
		price.setAsk(1.5);
		price.setLow(1.3);
		price.setHigh(1.9);
		price.setDirection(5);
		price.setDigits(8);
		price.setSpread(7);
		price.setModify_time(DateUtil.getTimestamp(new Date()));	
		prices.add(price);
		
	//	int statusCode = priceDao.addPrice(price);
		int statusCode = priceDao.addPrices(prices);	
		System.out.println(statusCode);
	}

}
