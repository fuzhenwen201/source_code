package com.broctagon.trad.rm.db.test;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.db.SpringBootApp;
import com.broctagon.trad.rm.db.dao.OrderDao;
import com.broctagon.trad.rm.db.model.Order;

@RunWith(SpringRunner.class)
@SpringBootTest(classes=SpringBootApp.class)
public class DbOrderDaoTest {

	private Logger logger = Logger.getLogger(DbOrderDaoTest.class);
	
	@Autowired
	private OrderDao orderDao;
	
	public void InsertOrder(){
		
		List<Order> orderList = new ArrayList<Order>();
		for(int i=0; i<2; i++){			
			Order order = new Order();
			order.setReqid("4e978baf75d54a5d8bca9c0491f9ff9d");
			order.setGroupid(1);
			order.setClientid(83);
			order.setOrderstatus(0);
			order.setAssetid(1);
			order.setDirection(0);
			order.setAmount(100);
			order.setExpiryperiod(0);
			long opentime = System.currentTimeMillis();
			order.setOpentime(opentime);
			order.setOpenprice(729000);
			order.setCloseprice(72);
			order.setPayout(0);
			order.setSgid(1);
			order.setTeid(0);
			order.setExpirytime(opentime+15000);
			order.setComment("comment test");
			order.setType(4);
			UUID uuid = UUID.randomUUID();
			String uuidStr = uuid.toString().replaceAll("-", "");	
			order.setOrderid(uuidStr);
			String orderStr = JSON.toJSONString(order);
			logger.info("orderStr:"+orderStr);
						
			orderList.add(order);		
		}

		String orderListStr = JSON.toJSONString(orderList);
		logger.info("orderListStr:" + orderListStr);
		int reslut = orderDao.addOrders(orderList);
		logger.info("reslut:" + reslut);
	}
	
	public void insertInOpenposition(){
		
	}
	
	@Test
	public void QueryOrder(){
		List<Order> allOrderList = orderDao.queryAllOrders();
		logger.info("JSON.toJSONString(allOrderList):" + JSON.toJSONString(allOrderList));
		long currentTime = (long)1505878039;
		currentTime = currentTime*1000*1000;
		System.out.println("currentTime:" + currentTime);
		List<Order> allOpenOrderList =orderDao.queryAllOpenOrdersInExpiryTime(currentTime);
		logger.info("JSON.toJSONString(allOpenOrderList):" + JSON.toJSONString(allOpenOrderList));
	}
	@Test
	public void QueryOrderByClientId(){
		List<Order> allOrderList = orderDao.queryOrdersByClientId(1);
		logger.info("JSON.toJSONString(allOrderList):" + JSON.toJSONString(allOrderList));
		
		List<Order> allOpenOrderList = orderDao.queryOpenOrdersByClientId(1);
		logger.info("JSON.toJSONString(allOpenOrderList):" + JSON.toJSONString(allOpenOrderList));
		
		List<Order> allClosedOrderList = orderDao.queryClosedOrdersByClientId(1);
		logger.info("JSON.toJSONString(allClosedOrderList):" + JSON.toJSONString(allClosedOrderList));
		
		List<Order> latest30List = orderDao.queryLatest30ClosedAndBalanceOrdersByClientId(1);
		logger.info("JSON.toJSONString(latest30List):" + JSON.toJSONString(latest30List));
	}
	@Test
	public void QueryOrdersByOrderStatus(){
		List<Order> orderList = orderDao.queryOrdersByStatus(0);
		logger.info("JSON.toJSONString(orderList):" + JSON.toJSONString(orderList));
	}
	
}
