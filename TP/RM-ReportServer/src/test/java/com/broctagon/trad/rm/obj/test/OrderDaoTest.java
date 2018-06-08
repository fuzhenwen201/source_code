package com.broctagon.trad.rm.obj.test;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.db.dao.OrderDao;
import com.broctagon.trad.rm.db.model.Order;
import com.broctagon.trad.rm.obj.common.CommonUtil;

@Component
public class OrderDaoTest {

	private Logger logger = Logger.getLogger(OrderDaoTest.class);
	
	@Autowired
	private OrderDao orderDao;
	
	public List<Order> prepareOrders(){
		List<Order> orderList = new ArrayList<Order>();
		for(int i=0; i<2; i++){			
			Order order = new Order();
			order.setReqid("4e978baf75d54a5d8bca9c0491f9ff9d");
			order.setGroupid(1);
			order.setClientid(85);
			order.setOrderstatus(0);
			order.setAssetid(1);
			order.setDirection(0);
			order.setAmount(100);
			int expiryperiod = 5;
			order.setExpiryperiod(expiryperiod);
			long opentime = System.currentTimeMillis();
			order.setOpentime(opentime);
			order.setOpenprice(729000);
			
			if(i%2==0){
				order.setCloseprice(0);
			}
					
			order.setPayout(0);
			order.setSgid(1);
			order.setTeid(0);
			order.setExpirytime(opentime+ expiryperiod*1000*1000);	
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
		
/*		logger.info("orderList.size():"+orderList.size());
		for(int i=0; i<orderList.size(); i++){
			Order order2 = orderList.get(i);
			logger.info(JSON.toJSONString(order2));
			logger.info(order2.getOrderid());
		}*/
					
		return 	orderList;
		
	}
	
	public void insertOrder(List<Order> orderList){
		 orderDao.addOrders(orderList);
	}
	
	public void InsertOrder(){
		List<Order> orderList = prepareOrders();
		int result = orderDao.addOrders(orderList);
		logger.info(""+result);
	}
	
	public void QueryOrder(){
		List<Order> allOrderList = orderDao.queryAllOrders();
		logger.info("JSON.toJSONString(allOrderList):" + JSON.toJSONString(allOrderList));
		try {
			long startTime = CommonUtil.getIntradayStartNanos();
			List<Order> intradayOrderList = orderDao.queryOrdersByStartOpenTime(startTime);
			logger.info("JSON.toJSONString(intradayOrderList):" + JSON.toJSONString(intradayOrderList));
		} catch (ParseException e) {
			e.printStackTrace();
		}
		
	}
	
	
	public void QueryOrdersByOrderStatus(int orderStatussss){
		List<Order> orderList = orderDao.queryOrdersByStatus(orderStatussss);
		logger.info("JSON.toJSONString(orderList):" + JSON.toJSONString(orderList));
	}
	
}
