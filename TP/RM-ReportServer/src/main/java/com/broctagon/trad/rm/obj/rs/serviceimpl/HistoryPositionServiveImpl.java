package com.broctagon.trad.rm.obj.rs.serviceimpl;

import java.text.ParseException;
import java.util.Collections;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.db.dao.OrderDao;
import com.broctagon.trad.rm.db.model.Order;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.common.CommonUtil;
import com.broctagon.trad.rm.obj.message.OrdersAns;
import com.broctagon.trad.rm.obj.message.OrdersReq;
import com.broctagon.trad.rm.obj.rs.rabbitmq.RabbitMQHelper;
import com.broctagon.trad.rm.obj.rs.service.HistoryPositionService;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

@Service
public class HistoryPositionServiveImpl implements HistoryPositionService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired private ShardedJedisPool shardedJedisPool;
	@Autowired private RabbitMQHelper rabbitMQHelper;
	@Autowired private OrderDao orderDao;
	
	public void responseGetPositionsReq(byte[] content){
		String contentStr = new String(content);
		logger.info("new String(content):" + contentStr);
		
		OrdersReq ordersReq = JSON.parseObject(contentStr, OrdersReq.class);		
		if(ordersReq==null){
			logger.error("ordersReq==null");
			return;
		}
		
		List<Order> orderList = getIntradayPositions();
		if (orderList.size() > 0) {
			String getPositionAnsStr = getordersAnsStr(orderList, ordersReq);
			rabbitMQHelper.publishGetPositionAns(getPositionAnsStr.getBytes());
		} else {
			logger.info("orderList is empty");
		}
	}
	
	public List<Order> getIntradayPositions(){
		List<Order> orderList = Collections.emptyList();
		try {
			long startTime = CommonUtil.getIntradayStartNanos();
			orderList = orderDao.queryOrdersByStartOpenTime(startTime);
		} catch (ParseException e) {
			logger.info(e.getMessage());
		} catch (Exception ex) {
			logger.info(ex.getMessage());
		}
		return orderList;
	}
	
	public void responseGetAllOpenPosition(byte[] content){
		String contentStr = new String(content);
		logger.info("new String(content):" + contentStr);
		
		OrdersReq ordersReq = JSON.parseObject(contentStr, OrdersReq.class);		
		if(ordersReq==null){
			logger.error("ordersReq==null");
			return;
		}
		
		List<Order> orderList = getAllOpenPosition();	
		String getPositionAnsStr = getordersAnsStr(orderList, ordersReq);
		rabbitMQHelper.publishGetAllOpenPositionAns(getPositionAnsStr.getBytes(), ordersReq.getSgid());	
	}
	
	public List<Order> getAllOpenPosition(){
		List<Order> orderList = Collections.emptyList();
		ShardedJedis shardedJedis = null;
		try {
			shardedJedis = shardedJedisPool.getResource();
			String orders = shardedJedis.get(CommonConstants.REDIS_KEY_RM_OPENING_POSITION);
			if(!CommonUtil.isEmptyString(orders)){
				orderList = JSON.parseArray(orders, Order.class);
			}			
		} catch (Exception ex) {
			logger.error(ex.getMessage());
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}
		return orderList;
	}
	
	private String getordersAnsStr(List<Order> orderList, OrdersReq ordersReq) {
		OrdersAns ordersAns = new OrdersAns();
		ordersAns.setBody(orderList);
		ordersAns.setClientid(ordersReq.getClientid());
		ordersAns.setManagerid(ordersReq.getManagerid());
		ordersAns.setSgid(ordersReq.getSgid());
		ordersAns.setReqid(ordersReq.getReqid());
		ordersAns.setSession(ordersReq.getSession());
		ordersAns.setTotal(orderList.size());
		String ordersAnsStr = JSON.toJSONString(ordersAns);
		logger.info("ordersAnsStr:" + ordersAnsStr);
		return ordersAnsStr;
	}
	
}
