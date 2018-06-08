package com.broctagon.trad.rm.src.serviceimpl;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.src.common.CommonConstants;
import com.broctagon.trad.rm.src.message.UpdateBalanceAns;
import com.broctagon.trad.rm.src.message.UserInfo;
import com.broctagon.trad.rm.src.model.BalanceTrader;
import com.broctagon.trad.rm.src.service.UserInfoService;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

@Service
public class UserInfoServiceImpl implements UserInfoService{
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private ShardedJedisPool shardedJedisPool;
	
	public void storeUserInfo(byte[] content){	
		if(content==null||content.length==0){
			logger.error("content==null||content.length==0");
			return;
		}		
		String contentStr = new String(content);
		logger.info("contentStr:" + contentStr);
		
		UserInfo userInfo = JSON.parseObject(contentStr, UserInfo.class);
		if(userInfo==null||userInfo.getClientid()==0){
			logger.error("userInfo==null||userInfo.getClientid()==0,contentStr:"+contentStr);
			return;
		}
		
		BalanceTrader updateTrader = new BalanceTrader();
		updateTrader.setClientid(userInfo.getClientid());
		updateTrader.setGroupid(userInfo.getGroupid());
		updateTrader.setPrevbalance(userInfo.getPrevbalance());
		updateTrader.setMargin(userInfo.getMargin());
		updateTrader.setFreemargin(userInfo.getFreemargin());
		updateTrader.setBalance(userInfo.getFreemargin()+userInfo.getMargin());

		writeToRedis(updateTrader);						
	}
		
	public void changeUserBalance(byte[] content){
		if(content==null||content.length==0){
			logger.error("content==null||content.length==0");
			return;
		}		
		String contentStr = new String(content);
		logger.info("contentStr:" + contentStr);
		
		UpdateBalanceAns changeBalanceAns = JSON.parseObject(contentStr,UpdateBalanceAns.class);
		if(changeBalanceAns==null||changeBalanceAns.getAccountid()==0){
			logger.error("changeBalanceAns==null||changeBalanceAns.getAccountid()==0,contentStr:"+contentStr);
			return;
		}
		
		BalanceTrader updateTrader = new BalanceTrader();
		updateTrader.setClientid(changeBalanceAns.getAccountid());
		updateTrader.setMargin(0);
		updateTrader.setFreemargin(changeBalanceAns.getBalance());
		updateTrader.setBalance(changeBalanceAns.getBalance());	
	}
	
	private void writeToRedis(BalanceTrader updateTrader){	
		String updateTraderStr = JSON.toJSONString(updateTrader);
		logger.info("updateTraderStr:"+updateTraderStr);
		
		String traderKey = String.valueOf(updateTrader.getClientid());
		ShardedJedis shardedJedis = null;
		try{
			shardedJedis = shardedJedisPool.getResource();
			shardedJedis.hset(CommonConstants.REDIS_KEY_RM_HASH_USERINFO, traderKey, updateTraderStr);
			shardedJedis.sadd(CommonConstants.REDIS_KEY_RM_SET_USERINFO, traderKey);
		}
		catch(Exception ex){
			logger.error(ex.getMessage());
		}
		finally{
			if(shardedJedis!=null){
				shardedJedis.close();
			}
		}							
	}
	
}
