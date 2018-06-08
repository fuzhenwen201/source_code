package com.broctagon.trad.rm.obj.rs.serviceimpl;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Set;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.message.BalanceTradersAns;
import com.broctagon.trad.rm.obj.model.BalanceTrader;
import com.broctagon.trad.rm.obj.rs.rabbitmq.RabbitMQHelper;
import com.broctagon.trad.rm.obj.rs.service.UpdateTraderService;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPipeline;
import redis.clients.jedis.ShardedJedisPool;

@Service
public class UpdateTraderServiceImpl implements UpdateTraderService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private ShardedJedisPool shardedJedisPool;
	@Autowired 
	private RabbitMQHelper rabbitMQHelper;
	
	public void broadcastUpdateTrader(){	
		List<BalanceTrader> balanceTraderList = fetchUpdateBalanceTrader();	
		if(balanceTraderList!=null&&balanceTraderList.size()>0){
			String updateTraderAnsStr = getUpdateBalanceAnsStr(balanceTraderList);
			rabbitMQHelper.publishUpdateTrader(updateTraderAnsStr.getBytes());
		}	
	}
		
	public List<BalanceTrader> fetchUpdateBalanceTrader(){
		ShardedJedis shardedJedis = null;
		try{			
			shardedJedis = shardedJedisPool.getResource();
			List<BalanceTrader> updateTraderList = new ArrayList<BalanceTrader>();
			
			Set<String> userInfoSet =shardedJedis.smembers(CommonConstants.REDIS_KEY_RM_SET_USERINFO);
			if(userInfoSet==null||userInfoSet.isEmpty()){
				return null;
			}
			shardedJedis.del(CommonConstants.REDIS_KEY_RM_SET_USERINFO);
			ShardedJedisPipeline pipeline = shardedJedis.pipelined();
			for(String userInfoStr : userInfoSet){
				pipeline.hget(CommonConstants.REDIS_KEY_RM_HASH_USERINFO, userInfoStr);
			}
			List<Object> list = pipeline.syncAndReturnAll();
			if(list==null||list.isEmpty()){
				logger.info("Empty from redis key : " + CommonConstants.REDIS_KEY_RM_SET_USERINFO);
				return Collections.emptyList();
			}
			
			for(Object obj: list){
				String updateTraderStr = (String)obj;
				BalanceTrader updateTrader = JSON.parseObject(updateTraderStr, BalanceTrader.class);
				updateTraderList.add(updateTrader);
			}
			return updateTraderList;
		}
		catch(Exception ex){
			logger.error(ex.getMessage());
			return Collections.emptyList();
		}
		finally{
			if(shardedJedis!=null){
				shardedJedis.close();
			}
		}
	}
	
	public void publishOnlineTraderBalance(){	
		
		List<BalanceTrader> balanceTraderList = fetchOnlineBalanceTrader();	
		if(balanceTraderList!=null&&balanceTraderList.size()>0){
			String updateTraderAnsStr = getUpdateBalanceAnsStr(balanceTraderList);
			rabbitMQHelper.publishUpdateTrader(updateTraderAnsStr.getBytes());
		}	
	}
	
	public List<BalanceTrader> fetchOnlineBalanceTrader(){					
		ShardedJedis shardedJedis = null;
		try{			
			shardedJedis = shardedJedisPool.getResource();
			
			List<BalanceTrader> balanceTraders = new ArrayList<BalanceTrader>();
			
			Set<String> onlineClientidSet = shardedJedis.smembers(CommonConstants.REDIS_KEY_RM_SET_ONLINE_USERS);	
			
			ShardedJedisPipeline pipeline = shardedJedis.pipelined();
			for(String onlineClientid: onlineClientidSet){
				pipeline.hget(CommonConstants.REDIS_KEY_RM_HASH_USERINFO, onlineClientid);
			}
			List<Object> onlineBalanceTraderObjs = pipeline.syncAndReturnAll();
			for(Object onlineBalanceTraderObj: onlineBalanceTraderObjs){
				if(onlineBalanceTraderObj!=null){
					String balanceTraderStr = (String)onlineBalanceTraderObj;
					logger.info("balanceTraderStr" + balanceTraderStr);
					BalanceTrader balanceTrader = JSON.parseObject(balanceTraderStr, BalanceTrader.class);
					balanceTraders.add(balanceTrader);
				}
			} 
			
			logger.info("JSON.toJSONString(balanceTraders):" + JSON.toJSONString(balanceTraders));
			
			return balanceTraders;
		}
		catch(Exception ex){
			logger.error(ex.getMessage());
			return Collections.emptyList();
		}
		finally{
			if(shardedJedis!=null){
				shardedJedis.close();
			}
		}
	}
	
	private String getUpdateBalanceAnsStr(List<BalanceTrader> balanceTraderList){
		BalanceTradersAns updateTraderAns = new BalanceTradersAns();
		updateTraderAns.setTraderlist(balanceTraderList);			
    	UUID uuid = UUID.randomUUID();
    	String reqid = uuid.toString().replace("-", "");
    	updateTraderAns.setReqid(reqid);
    	String updateTraderAnsStr = JSON.toJSONString(updateTraderAns);	    	
    	logger.info("updateTraderAnsStr:" + updateTraderAnsStr);	
    	return updateTraderAnsStr;
	}
		
	
}
