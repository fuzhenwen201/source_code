
package com.broctagon.trad.rm.src.serviceimpl;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.src.common.CommonConstants;
import com.broctagon.trad.rm.src.message.OnlineTraderAns;
import com.broctagon.trad.rm.src.message.OnlineUserAns;
import com.broctagon.trad.rm.src.message.UserInfo;
import com.broctagon.trad.rm.src.service.OnlineUserSetService;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPipeline;
import redis.clients.jedis.ShardedJedisPool;

/**
* @auther: Water
* @time: Oct 9, 2017 6:06:09 PM
* 
*/

@Service
public class OnlineUserSetServiceImpl implements OnlineUserSetService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private ShardedJedisPool shardedJedisPool;
	/*@Autowired
	private RabbitMQHelper rabbitMQHelper;
	*/
	private boolean initOnlineUserFlag = false;
	
	public void requestAllOnlineUsers(){
	//	rabbitMQHelper.publishOnlineUsersRequest("".getBytes());
	}
	
	public void addOnlineUser(byte[] content){	
		String userInfoStr = new String(content);
		logger.info(userInfoStr);
		
		UserInfo userInfo = JSON.parseObject(userInfoStr, UserInfo.class);
		int clientid = userInfo.getClientid();
		if(clientid!=0){
			ShardedJedis shardedJedis=null;	
			try{
				shardedJedis=shardedJedisPool.getResource();
				shardedJedis.sadd(CommonConstants.REDIS_KEY_RM_SET_ONLINE_USERS, ""+clientid);
			}
			catch(Exception e){
				logger.error(e.getMessage());
			}
			finally{
				if(shardedJedis!=null){
					shardedJedis.close();
				}
			}
		}
	}
	
	public void removeOnlineUser(byte[] content){	
		String userInfoStr = new String(content);
		logger.info(userInfoStr);
		
		UserInfo userInfo = JSON.parseObject(userInfoStr, UserInfo.class);
		int clientid = userInfo.getClientid();
		if(clientid!=0){
			ShardedJedis shardedJedis=null;	
			try{
				shardedJedis=shardedJedisPool.getResource();
				shardedJedis.srem(CommonConstants.REDIS_KEY_RM_SET_ONLINE_USERS, ""+clientid);
			}
			catch(Exception e){
				logger.error(e.getMessage());
			}
			finally{
				if(shardedJedis!=null){
					shardedJedis.close();
				}
			}
		}
	}
	
	public void storeAllOnlineUsers(byte[] content){	
		
		if(initOnlineUserFlag){
			return;
		}
		else{
			initOnlineUserFlag=true;
		}
		
		String onlineUserAns = new String(content);
		logger.info(onlineUserAns);
		
		OnlineTraderAns onlineTraderAns = JSON.parseObject(onlineUserAns,OnlineTraderAns.class);
		List<OnlineUserAns> onlineUsers = onlineTraderAns.getOnlineuserlist();
		Set<Integer> onlineClinetIdSet = new HashSet<Integer>();
		for(int i=0; i<onlineUsers.size(); i++){
			onlineClinetIdSet.add(onlineUsers.get(i).getClientid());
		}
		
		ShardedJedis shardedJedis=null;	
		try{
			shardedJedis=shardedJedisPool.getResource();
			ShardedJedisPipeline pipeline = shardedJedis.pipelined();
			for(Integer clientid: onlineClinetIdSet){
				pipeline.sadd(CommonConstants.REDIS_KEY_RM_SET_ONLINE_USERS, ""+clientid);
			}
			pipeline.sync();
		}
		catch(Exception e){
			logger.error(e.getMessage());
		}
		finally{
			if(shardedJedis!=null){
				shardedJedis.close();
			}
		}
		
	}
	
}
