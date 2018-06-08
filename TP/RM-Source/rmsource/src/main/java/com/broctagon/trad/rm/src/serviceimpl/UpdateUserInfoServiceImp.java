/**
 * 
 */
package com.broctagon.trad.rm.src.serviceimpl;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.src.common.CommonConstants;
import com.broctagon.trad.rm.src.message.UserInfoMsg;
import com.broctagon.tradstnode.rabbit.message.Message;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

/**
 * @author imdadullah
 *
 */
@Service
public class UpdateUserInfoServiceImp {
	private Logger LOGGER = Logger.getLogger(this.getClass());

	@Autowired
	private ShardedJedisPool shardedJedisPool;

	public void writeUserInfoToRedis(Message message) {
		UserInfoMsg userInfoMsg = null;
		String messageString = message.getMessageString();
		LOGGER.info("MESSAGE RECEIVED FOR USER INFO >>>>>>>>>>>>>>>>" + messageString);
		userInfoMsg = JSON.parseObject(messageString.trim(), UserInfoMsg.class);
		ShardedJedis shardedJedis = null;
		try {
			String clientId = String.valueOf(userInfoMsg.getClientid());
			shardedJedis = shardedJedisPool.getResource();
			shardedJedis.hset(CommonConstants.REDIS_KEY_RM_HASH_UPDATE_USERINFO, clientId, messageString);
			shardedJedis.sadd(CommonConstants.REDIS_KEY_RM_SET_UPDATE_USERINFO, clientId);
		} catch (Exception ex) {
			LOGGER.error(ex.getMessage());
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}

	}
}
