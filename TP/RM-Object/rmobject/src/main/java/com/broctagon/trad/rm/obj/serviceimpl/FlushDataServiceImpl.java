/**
 * 
 */
package com.broctagon.trad.rm.obj.serviceimpl;


import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.db.dao.CloseTradeDao;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.service.FlushDataService;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

/**
 * @author imdadullah
 *
 */
@Service
public class FlushDataServiceImpl implements FlushDataService{

	private Logger LOGGER = Logger.getLogger(this.getClass());
	@Autowired
	private CloseTradeDao closeTradeDao;
	@Autowired
	private ShardedJedisPool shardedJedisPool;
	
	public void deleteAllFromClosePositionTbl(){
		LOGGER.info("<<<<<<<<<<<<< GOING TO DELETE ALL DATA FROM CLOSED POSITION TABLE >>>>>>>>>>>>");
		closeTradeDao.deleteAllCloseTrades();
		LOGGER.info("<<<<<<<<<<<<< ALL DATA DELETED SUCCESSFULLY FROM CLOSED POSITION TABLE >>>>>>>>>>>>");
	}
	
	public void flushRedisData(){
		ShardedJedis shardedJedis = null;
		try {
			shardedJedis = shardedJedisPool.getResource();
			LOGGER.info("############ GOING TO FLUSH REDIS DATA ############");
			shardedJedis.del(CommonConstants.REDIS_KEY_RM_HASH_OPENUP_ORDERANS);
			shardedJedis.del(CommonConstants.REDIS_KEY_RM_HASH_OPENDOWN_ORDERANS);
			shardedJedis.hdel(CommonConstants.REDIS_KEY_RM_SYMBOLBO, "SymbolBo");
			LOGGER.info("########### REDIS DATA FLUSHED SUCCESSFULLY ###########");
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}
	}
}
