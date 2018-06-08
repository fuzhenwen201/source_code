package com.broctagon.trad.rm.src.serviceimpl;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.src.common.CommonConstants;
import com.broctagon.trad.rm.src.model.Order;
import com.broctagon.trad.rm.src.service.OrderAnsService;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

@Service
public class OrderAnsServiceImpl implements OrderAnsService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private ShardedJedisPool shardedJedisPool;
	
	public void storeOrderAns(byte[] orderAnsByte){		
		if(orderAnsByte==null||orderAnsByte.length==0){
			logger.info("orderAnsByte==null||orderAnsByte.length==0");
			return;
		}
		String orderAnsStr = new String(orderAnsByte);
		
		Order order = JSON.parseObject(orderAnsStr, Order.class);
		if(order==null||order.getOrderid()==null){
			logger.info("order==null,orderAnsStr:" + orderAnsStr);
			return;
		}
		logger.info("orderAnsStr:"+orderAnsStr);
	
		long currentTime = System.currentTimeMillis();
		if(order.getOpentime() < currentTime*100){
			order.setOpentime(order.getOpentime()*1000);
		}
		order.setExpirytime(order.getOpentime() + order.getExpiryperiod()*1000*1000);
		writeToRedis(order);
	}
	
	public void writeToRedis(Order order ){
		String orderStr = JSON.toJSONString(order);
		logger.info("orderStr:" + orderStr);

		ShardedJedis shardedJedis = null;
		try {
			shardedJedis = shardedJedisPool.getResource();
			shardedJedis.hset(CommonConstants.REDIS_KEY_RM_HASH_ORDERANS, order.getOrderid(), orderStr);
			
			// writen by imdad
			if (order.getDirection() == 1 && order.getOpenprice() >0) {
				if (order.getCloseprice() > 0) {
					shardedJedis.hset(CommonConstants.REDIS_KEY_RM_HASH_CLOSED_ORDERANS, order.getOrderid(), orderStr);
					shardedJedis.hdel(CommonConstants.REDIS_KEY_RM_HASH_OPENUP_ORDERANS, order.getOrderid());
				} else {
					shardedJedis.hset(CommonConstants.REDIS_KEY_RM_HASH_OPENUP_ORDERANS, order.getOrderid(), orderStr);
				}
			} else if (order.getDirection() == 0 && order.getOpenprice() >0) {
				if (order.getCloseprice() > 0) {
					shardedJedis.hset(CommonConstants.REDIS_KEY_RM_HASH_CLOSED_ORDERANS, order.getOrderid(), orderStr);
					shardedJedis.hdel(CommonConstants.REDIS_KEY_RM_HASH_OPENDOWN_ORDERANS, order.getOrderid());
				} else {
					shardedJedis.hset(CommonConstants.REDIS_KEY_RM_HASH_OPENDOWN_ORDERANS, order.getOrderid(),orderStr);
				}

			}

		} catch (Exception ex) {
			logger.error(ex.getMessage());
			System.out.println("Error Message"+ex);
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}
	}
	
}
