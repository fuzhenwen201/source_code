/**
 * 
 */
package com.broctagon.trad.rm.obj.serviceimpl;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.common.RMLogger;
import com.broctagon.trad.rm.obj.message.SymbolBoMsg;
import com.broctagon.trad.rm.db.model.Order;
import com.broctagon.trad.rm.obj.model.RedisSymbolBoModel;
import com.broctagon.trad.rm.obj.service.SymbolBoService;
import com.broctagon.tradstnode.rabbit.utils.ConstantUtils;
import com.broctagon.tradstnode.rabbit.utils.DateUtils;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

/**
 * @author imdadullah
 *
 */
@Service
public class SysmbolBoServiceImpl implements SymbolBoService {
	private Logger LOGGER = Logger.getLogger(this.getClass());
	@Autowired
	private ShardedJedisPool shardedJedisPool;
	@Autowired
	private UpdatedBoPublisherServiceImpl updatedBoPublisherServiceImpl;

	public List<TradeModel> getClosedTradeList(List<String> closedOrderList) {
		if (closedOrderList.size() == 0)
			return null;
		ShardedJedis shardedJedis = null;
		List<TradeModel> closedTradeList = new ArrayList<>();
		TradeModel tradeModel;
		try {
			shardedJedis = shardedJedisPool.getResource();
			for (String orderAns : closedOrderList) {
				RMLogger.writeLogMessage(DateUtils.convertDateToString(new Date(), DateUtils.DdMmYyyyHmsFormatWithZone),
						ConstantUtils.MESSAGE_IN, CommonConstants.MESSAGE_TAG_ORDERANS_RM, orderAns.length(),orderAns);
				Order order = (Order) JSON.parseObject(orderAns, Order.class);
				tradeModel = getTradeModel(order);
				closedTradeList.add(tradeModel);
				shardedJedis.hdel(CommonConstants.REDIS_KEY_RM_HASH_CLOSED_ORDERANS, tradeModel.getOrderid());
			}
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}

		return closedTradeList;
	}

	@Override
	public void writeSymbolBoToRedis(List<TradeModel> closedTradeList) {
		ShardedJedis shardedJedis = null;
		Map<Integer, SymbolBoMsg> processedSymbolBoMap = null;
		RedisSymbolBoModel redisBoModel = new RedisSymbolBoModel();
		if (closedTradeList != null) {
			processedSymbolBoMap = processForSymbolBo(closedTradeList);
		}
		try {
			shardedJedis = shardedJedisPool.getResource();
			String symbolRedisStr = shardedJedis.hget(CommonConstants.REDIS_KEY_RM_SYMBOLBO, "SymbolBo");
			if (symbolRedisStr != null) {
				RedisSymbolBoModel redisBo = JSON.parseObject(symbolRedisStr, RedisSymbolBoModel.class);
				processedSymbolBoMap = addSymbolBos(processedSymbolBoMap, redisBo.getBoMap());
			}
			redisBoModel.setBoMap(processedSymbolBoMap);
			String symbolBoStr = JSON.toJSONString(redisBoModel);
			LOGGER.info("@@@@@@@@@@@@ GOING TO WRITE SYMBOL BO TO REDIS @@@@@@@@@@@@@@");
			shardedJedis.hset(CommonConstants.REDIS_KEY_RM_SYMBOLBO, "SymbolBo", symbolBoStr);
			updatedBoPublisherServiceImpl.publishUpdatedSymbolBO(processedSymbolBoMap, closedTradeList);
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}

	}

	public Map<Integer, SymbolBoMsg> processForSymbolBo(List<TradeModel> tradeList) {
		Map<Integer, SymbolBoMsg> symbolBoMap = new HashMap<>();
		Map<Integer, List<TradeModel>> closedTradeList = tradeList.stream()
				.collect(Collectors.groupingBy(o -> o.getAssetid()));
		try {
			closedTradeList.entrySet().forEach(p -> {
				long sumUpVolume = 0;
				long sumdwVolume = 0;
				List<TradeModel> tlist = p.getValue();
				int assetId = p.getKey();
				if (tlist.size() > 0) {
					Map<Integer, List<TradeModel>> tradesMap = tlist.stream()
							.collect(Collectors.groupingBy(o -> o.getDirection()));
					List<TradeModel> listup = tradesMap.get(1);
					List<TradeModel> listdw = tradesMap.get(0);
					if (listup != null) {
						sumUpVolume = listup.stream().mapToLong(o -> o.getAmount()).sum();
					}
					if (listdw != null) {
						sumdwVolume = listdw.stream().mapToLong(k -> k.getAmount()).sum();
					}
					SymbolBoMsg symbolBoMsg = new SymbolBoMsg();
					symbolBoMsg.setAssetid(assetId);
					symbolBoMsg.setTotalupvolume(sumUpVolume);
					symbolBoMsg.setTotaldownvolume(sumdwVolume);
					symbolBoMsg.setNetvolume(sumUpVolume - sumdwVolume);
					symbolBoMap.put(assetId, symbolBoMsg);
				}
			});
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}
		return symbolBoMap;
	}

	public Map<Integer, SymbolBoMsg> addSymbolBos(Map<Integer, SymbolBoMsg> processedBoMap,
			Map<Integer, SymbolBoMsg> redisBoMap) {
		Map<Integer, SymbolBoMsg> redisMap = redisBoMap;
		if (processedBoMap == null) {
			return redisBoMap;
		}
		try {
			processedBoMap.entrySet().forEach(p -> {
				SymbolBoMsg symbolBoMsg = new SymbolBoMsg();
				int symbolId = p.getKey();
				SymbolBoMsg processedBo = p.getValue();
				SymbolBoMsg redisSymbolBo = redisMap.get(symbolId);
				if (redisSymbolBo != null) {
					long totalUpVolume = processedBo.getTotalupvolume() + redisSymbolBo.getTotalupvolume();
					long totalDownVolume = processedBo.getTotaldownvolume() + redisSymbolBo.getTotaldownvolume();
					symbolBoMsg.setAssetid(symbolId);
					symbolBoMsg.setTotalupvolume(totalUpVolume);
					symbolBoMsg.setTotaldownvolume(totalDownVolume);
					symbolBoMsg.setNetvolume(totalUpVolume - totalDownVolume);
				} else {
					symbolBoMsg = processedBo;
				}
				redisBoMap.put(symbolId, symbolBoMsg);
			});

		} catch (Exception e) {
			e.printStackTrace();
		}
		return redisBoMap;
	}

	public TradeModel getTradeModel(Order order) {
		TradeModel trade = new TradeModel();
		trade.setReqid(order.getReqid());
		trade.setGroupid(order.getGroupid());
		trade.setClientid(order.getClientid());
		trade.setOrderid(order.getOrderid());
		trade.setOrderstatus(order.getOrderstatus());
		trade.setAssetid(order.getAssetid());
		trade.setDirection(order.getDirection());
		trade.setAmount(order.getAmount());
		trade.setExpiryperiod(order.getExpiryperiod());
		long exp = (order.getOpentime() + (order.getExpiryperiod() * 1000000));
		trade.setExpirytime(exp);
		trade.setOpentime(order.getOpentime());
		trade.setOpenprice(order.getOpenprice());
		trade.setCloseprice(order.getCloseprice());
		trade.setPayout(order.getPayout());
		trade.setSgid(order.getSgid());
		trade.setTeid(order.getTeid());
		return trade;
	}

}
