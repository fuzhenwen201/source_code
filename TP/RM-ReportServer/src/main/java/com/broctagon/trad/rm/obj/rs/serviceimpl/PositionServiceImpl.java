package com.broctagon.trad.rm.obj.rs.serviceimpl;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.db.dao.OrderDao;
import com.broctagon.trad.rm.obj.message.OrdersAns;
import com.broctagon.trad.rm.obj.rs.rabbitmq.RabbitMQHelper;
import com.broctagon.trad.rm.obj.rs.service.PositionService;
import com.broctagon.trad.rm.db.model.Order;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

@Service
public class PositionServiceImpl implements PositionService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired private ShardedJedisPool shardedJedisPool;
	@Autowired private RabbitMQHelper rabbitMQHelper;	
	@Autowired private OrderDao orderDao;
	
	public void updatePositions(){						
		List<Order> orderList = getLatestOrders();
														
		if(orderList.size()>0){			
			String postionsAnsStr = getOrderAns(orderList);
			rabbitMQHelper.publishUpdatePositionsAns(postionsAnsStr.getBytes());						
			int result = orderDao.addOrders(orderList);		
			logger.info("addOrders result:" + result);
		}
		
		stortAllOpenPositionToRedis();
	}
	
	public List<Order> getLatestOrders(){
		List<Order> orderList = new ArrayList<Order>();		
		ShardedJedis shardedJedis = null;
		try{			
			shardedJedis = shardedJedisPool.getResource();		
			List<String> redisOrders = shardedJedis.hvals(CommonConstants.REDIS_KEY_RM_HASH_ORDERANS);	
			shardedJedis.del(CommonConstants.REDIS_KEY_RM_HASH_ORDERANS);
			for(String value: redisOrders){
				Order order = JSON.parseObject(value,Order.class);
				orderList.add(order);									
			}
		}
		catch(Exception ex){
			logger.error(ex.getMessage());	
		}
		finally{
			if(shardedJedis!=null){
				shardedJedis.close();
			}
		}
		return orderList;
	}
		
	public void stortAllOpenPositionToRedis() {
		String orders = "";
		try {
			int bufferTime = 2000; // millis
			long currentTime = (System.currentTimeMillis() - bufferTime) * 1000;
		//	logger.info("(System.currentTimeMillis()+1000)*1000:" + currentTime);
			List<Order> orderList = orderDao.queryAllOpenOrdersInExpiryTime(currentTime);
			orders = JSON.toJSONString(orderList);
		} catch (Exception ex) {
			logger.info(ex.getMessage());
		}

		ShardedJedis shardedJedis = null;
		try {
			shardedJedis = shardedJedisPool.getResource();
			shardedJedis.set(CommonConstants.REDIS_KEY_RM_OPENING_POSITION, orders);
		} catch (Exception ex) {
			logger.error(ex.getMessage());
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}
	}
	
	public String getOrderAns(List<Order> orderList){
		OrdersAns ordersAns = new OrdersAns();
		ordersAns.setBody(orderList);			
    	UUID uuid = UUID.randomUUID();
    	String reqid = uuid.toString().replace("-", "");
    	ordersAns.setReqid(reqid);
    	ordersAns.setTotal(orderList.size());
    	String ordersAnsStr = JSON.toJSONString(ordersAns);	    	
    	logger.info("ordersAnsStr:" + ordersAnsStr);
    	
    	return ordersAnsStr;
	}
	
}
