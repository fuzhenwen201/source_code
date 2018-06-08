/**
 * 
 */
package com.broctagon.trad.rm.obj.serviceimpl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.common.BoCommons;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.message.CustomBoMsg;
import com.broctagon.trad.rm.obj.model.RedisCustomizedBoModel;
import com.broctagon.trad.rm.obj.service.CustomizeBoService;


import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

/**
 * @author imdadullah
 *
 */
@Service
public class CustomizeBoServiceImpl implements CustomizeBoService {
	private Logger LOGGER = Logger.getLogger(this.getClass());
	@Autowired
	private ShardedJedisPool shardedJedisPool;
	@Autowired
	private UpdatedBoPublisherServiceImpl commonSerice;

	@Override
	public void writeCustomizedUpBoToRedis(List<TradeModel> openUpTradeList, List<TradeModel> openUpListForUpdate,
			List<TradeModel> closeTradeList) {
		RedisCustomizedBoModel redisCustomBo = new RedisCustomizedBoModel();
		ShardedJedis shardedJedis = null;
		Map<Integer, CustomBoMsg> customBoMap = null;
		if (openUpTradeList.size() > 0) {
			customBoMap = processForCustomizedBo(openUpTradeList, 1);
		}
		commonSerice.publishUpdatedCustomBO(customBoMap, openUpListForUpdate, closeTradeList, 1);
		try {
			shardedJedis = shardedJedisPool.getResource();
			redisCustomBo.setBoMap(customBoMap);
			String customUpBoStr = JSON.toJSONString(redisCustomBo);
			LOGGER.info("########### GOING TO WRITE CUSTOME BO UP TO REDIS ################"+customUpBoStr);
			shardedJedis.hset(CommonConstants.REDIS_KEY_RM_CUSTOMBO_UP, "CustomUpBo", customUpBoStr);
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}
	}

	@Override
	public void writeCustomizedDownBoToRedis(List<TradeModel> openDownTradeList, List<TradeModel> openDownListForUpdate,
			List<TradeModel> closeTradeList) {
		RedisCustomizedBoModel redisCustomBo = new RedisCustomizedBoModel();
		ShardedJedis shardedJedis = null;
		Map<Integer, CustomBoMsg> customBoMap = null;
		if (openDownTradeList.size() > 0) {
			customBoMap = processForCustomizedBo(openDownTradeList, 0);
		}
		commonSerice.publishUpdatedCustomBO(customBoMap, openDownListForUpdate, closeTradeList, 0);
		try {
			shardedJedis = shardedJedisPool.getResource();
			redisCustomBo.setBoMap(customBoMap);
			String customDownBoStr = JSON.toJSONString(redisCustomBo);
			LOGGER.info("########### GOING TO WRITE CUSTOME BO DOWN TO REDIS ################"+customDownBoStr);
			shardedJedis.hset(CommonConstants.REDIS_KEY_RM_CUSTOMBO_DOWN, "CustomDownBo", customDownBoStr);
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}

	}

	public Map<Integer, CustomBoMsg> processForCustomizedBo(List<TradeModel> tradeList,int direction) {
		Map<Integer, CustomBoMsg> customizeBoMap = new HashMap<>();
		Map<Integer, List<TradeModel>> map = tradeList.stream().collect(Collectors.groupingBy(o -> o.getAssetid()));
		if (map.isEmpty())
			return null;
		map.entrySet().forEach(o -> {
			try {
				CustomBoMsg customBoMsg = new CustomBoMsg();
				List<TradeModel> tlist = o.getValue();
				int assetId = o.getKey();
				int totalNoOfOrder = tlist.size();
				long totalVolume = tlist.stream().mapToLong(p -> p.getAmount()).sum();
				double averagePrice = tlist.stream().mapToDouble(p -> p.getOpenprice()).average().orElse(-1);
				double lowestPrice = tlist.stream().mapToDouble(p -> p.getOpenprice()).min().orElse(-1);
				double averageVolume = tlist.stream().mapToDouble(p -> p.getAmount()).average().orElse(-1);
				customBoMsg.setAssetid(assetId);
				customBoMsg.setAverageprice((long) averagePrice);
				customBoMsg.setAveragevolume((long) averageVolume);
				customBoMsg.setLowestprice((long) lowestPrice);
				customBoMsg.setTotalvolume((long) totalVolume);
				customBoMsg.setNooforders(totalNoOfOrder);
				customBoMsg.setScale(BoCommons.getScale(tlist));;
				customBoMsg.setDirection(direction);
				customizeBoMap.put(assetId, customBoMsg);
			} catch (Exception e) {
				LOGGER.error("ERROR FOLLOWED BY  : ", e);
				e.printStackTrace();
			}
		});
		return customizeBoMap;

	}
}
