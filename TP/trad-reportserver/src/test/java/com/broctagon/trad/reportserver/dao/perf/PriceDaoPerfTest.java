package com.broctagon.trad.reportserver.dao.perf;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.apache.log4j.DailyRollingFileAppender;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.validator.PublicClassValidator;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.trad.reportserver.config.DruidConfig;
import com.broctagon.trad.reportserver.dao.PriceDao;
import com.broctagon.trad.reportserver.model.Price;
import com.broctagon.trad.reportserver.util.DateUtil;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = {DruidConfig.class})
public class PriceDaoPerfTest {

	@Autowired
	private PriceDao dao;
	
	private List<Price> prices;
	
	private Price price;
	
	private List<String> ids;
	
	@Before
	public void init(){
		prices = new ArrayList<>();
		ids = new ArrayList<>();
		for(int i = 0; i < 10000; i ++){
			Price price = new Price();
			price.setSymbol(String.valueOf(i));
			price.setTime(DateUtil.getTimestamp(new Date()));
			price.setBid(0.1 + i);
			price.setAsk(0.1 + i);
			price.setLow(0.1 + i);
			price.setHigh(0.1 + i);
			price.setDirection(i);
			price.setDigits(i);
			price.setSpread(i);
			price.setModify_time(DateUtil.getTimestamp(new Date()));
			prices.add(price);
			ids.add(price.getSymbol());
		}
		
		price = new Price();
		price.setSymbol("test");
		price.setTime(DateUtil.getTimestamp(new Date()));
		price.setBid(1);
		price.setAsk(1);
		price.setLow(1);
		price.setHigh(1);
		price.setDirection(1);
		price.setDigits(1);
		price.setSpread(1);
		price.setModify_time(DateUtil.getTimestamp(new Date()));
	}
	
//	@Test
	public void testAddOne(){
		long startTime = System.currentTimeMillis();
		dao.addPrice(price);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testAddAll(){
		long startTime = System.currentTimeMillis();
		dao.addPrices(prices);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testUpdateOne(){
		price.setAsk(2);
		long startTime = System.currentTimeMillis();
		dao.updatePrice(price);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
	@Test
	public void testUpdateAll(){
		for(Price price: prices){
			price.setAsk(Double.parseDouble(price.getSymbol()));
		}
		long startTime = System.currentTimeMillis();
		dao.updatePrices(prices);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testSelectOne(){
		long startTime = System.currentTimeMillis();
		Price price = dao.getPriceBySymbol("test");
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		System.out.println(price);
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testSelectAll(){
		long startTime = System.currentTimeMillis();
		List<Price> pricesTest = dao.getPricesBySymbols(ids);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testDeleteOne(){
		long startTime = System.currentTimeMillis();
		dao.deletePriceBySymbol("test");
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
	
//	@Test
	public void testDeleteAll(){
		long startTime = System.currentTimeMillis();
		dao.deletePricesBySymbols(ids);
		long duration = (System.currentTimeMillis() - startTime) / 1000;
		Assert.assertTrue(duration <= 1);
	}
}
