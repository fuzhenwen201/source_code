package com.broctagon.trad.reportserver.dao;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.trad.reportserver.config.DruidConfig;
import com.broctagon.trad.reportserver.model.Price;
import com.broctagon.trad.reportserver.util.DateUtil;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = {DruidConfig.class})
public class PriceDaoTest {
	
	@Autowired
	private PriceDao priceDao;

//	@Test
	public void testGetPriceBySymbol(){
		Price price = priceDao.getPriceBySymbol("AMERICA");
		System.out.println(price);
	}
	
//	@Test
	public void testGetPricesBySymbol(){
		List<String> symbols = new ArrayList<>();
		symbols.add("AMECRICA");
		symbols.add("AMERICAbo");
		symbols.add("ASIA");
		List<Price> prices = priceDao.getPricesBySymbols(symbols);
		System.out.println(prices);
	}
	
//	@Test
	public void testAddPrice(){
		Price price = new Price();
		price.setSymbol("TEST");
		price.setTime(DateUtil.getTimestamp(new Date()));
		price.setBid(1.1);
		price.setAsk(1.5);
		price.setLow(1.3);
		price.setHigh(1.8);
		price.setDirection(5);
		price.setDigits(8);
		price.setSpread(3);
		price.setModify_time(DateUtil.getTimestamp(new Date()));
		int statusCode = priceDao.addPrice(price);
		System.out.println(statusCode);
	}
	
//	@Test
	public void testAddPrices(){
		List<Price> prices = new ArrayList<>();		
		Price p1 = new Price();
		p1.setSymbol("ZZZ");
		p1.setTime(DateUtil.getTimestamp(new Date()));
		p1.setBid(15);
		p1.setAsk(15);
		p1.setLow(15);
		p1.setHigh(15);
		p1.setDirection(15);
		p1.setDigits(15);
		p1.setSpread(15);
		p1.setModify_time(DateUtil.getTimestamp(new Date()));
		
		Price p2 = new Price();
		p2.setSymbol("TEST");
		p2.setTime(DateUtil.getTimestamp(new Date()));
		p2.setBid(20);
		p2.setAsk(20);
		p2.setLow(20);
		p2.setHigh(20);
		p2.setDirection(20);
		p2.setDigits(20);
		p2.setSpread(20);
		p2.setModify_time(DateUtil.getTimestamp(new Date()));
		prices.add(p1);
		prices.add(p2);
		priceDao.addPrices(prices);
	}
	
//	@Test
	public void testUpdatePrice(){
		Price price = new Price();
		price.setSymbol("TEST");
		price.setTime(DateUtil.getTimestamp(new Date()));
		price.setBid(55);
		price.setAsk(55);
		price.setLow(55);
		price.setHigh(55);
		price.setDirection(55);
		price.setDigits(55);
		price.setSpread(55);
		price.setModify_time(DateUtil.getTimestamp(new Date()));
		priceDao.updatePrice(price);
	}
	
//	@Test
	public void testUpdatePrices(){
		List<Price> prices = new ArrayList<>();
		Price p1 = new Price();
		p1.setSymbol("d");
		p1.setTime(DateUtil.getTimestamp(new Date()));
		p1.setBid(55);
		p1.setAsk(55);
		p1.setLow(55);
		p1.setHigh(55);
		p1.setDirection(55);
		p1.setDigits(55);
		p1.setSpread(55);
		p1.setModify_time(DateUtil.getTimestamp(new Date()));
		Price p2 = new Price();
		p2.setSymbol("e");
		p2.setTime(DateUtil.getTimestamp(new Date()));
		p2.setBid(55);
		p2.setAsk(55);
		p2.setLow(55);
		p2.setHigh(55);
		p2.setDirection(55);
		p2.setDigits(55);
		p2.setSpread(55);
		p2.setModify_time(DateUtil.getTimestamp(new Date()));
		prices.add(p1);
		prices.add(p2);
		priceDao.updatePrices(prices);
	}
	
//	@Test
	public void testDeletePriceBySymbol(){
		priceDao.deletePriceBySymbol("d");
	}
	
//	@Test
	public void testDeletePricesBySymbols(){
		List<String> symbols = new ArrayList<>();
		symbols.add("e");
		symbols.add("m");
		priceDao.deletePricesBySymbols(symbols);
	}
}

