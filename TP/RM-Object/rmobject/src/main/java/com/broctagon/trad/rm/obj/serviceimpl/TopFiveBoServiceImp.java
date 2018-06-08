/**
 * 
 */
package com.broctagon.trad.rm.obj.serviceimpl;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.OptionalDouble;
import java.util.OptionalLong;
import java.util.stream.Collectors;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.db.model.Order;
import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.common.BoCommons;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.common.RMLogger;
import com.broctagon.trad.rm.obj.message.TopFiveBoMsg;
import com.broctagon.trad.rm.obj.model.RedisTopFiveBoModel;
import com.broctagon.trad.rm.obj.service.TopFiveBoService;
import com.broctagon.tradstnode.rabbit.utils.ConstantUtils;
import com.broctagon.tradstnode.rabbit.utils.DateUtils;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

/**
 * @author imdadullah
 *
 */
@Service
public class TopFiveBoServiceImp implements TopFiveBoService {
	private Logger LOGGER = Logger.getLogger(this.getClass());
	@Autowired
	private ShardedJedisPool shardedJedisPool;
	@Autowired
	private UpdatedBoPublisherServiceImpl updatedBoPublisherServiceImpl;

	@Override
	public List<TradeModel> getOpenUpTradeList(List<String> openUpOrderList) {
		List<TradeModel> openUpTradeList = new ArrayList<>();
		TradeModel tradeModel;
		for (String orderAns : openUpOrderList) {
			RMLogger.writeLogMessage(DateUtils.convertDateToString(new Date(), DateUtils.DdMmYyyyHmsFormatWithZone),
					ConstantUtils.MESSAGE_IN, CommonConstants.MESSAGE_TAG_ORDERANS_RM, orderAns.length(),orderAns);
			Order order = (Order) JSON.parseObject(orderAns, Order.class);
			tradeModel = getTradeModel(order);
			openUpTradeList.add(tradeModel);
		}
		return openUpTradeList;
	}

	@Override
	public List<TradeModel> getOpenDownTradeList(List<String> openDownOrderList) {
		List<TradeModel> openDownTradeList = new ArrayList<>();
		TradeModel tradeModel;
		for (String orderAns : openDownOrderList) {
			RMLogger.writeLogMessage(DateUtils.convertDateToString(new Date(), DateUtils.DdMmYyyyHmsFormatWithZone),
					ConstantUtils.MESSAGE_IN, CommonConstants.MESSAGE_TAG_ORDERANS_RM, orderAns.length(),orderAns);
			Order order = (Order) JSON.parseObject(orderAns, Order.class);
			tradeModel = getTradeModel(order);
			openDownTradeList.add(tradeModel);
		}
		return openDownTradeList;
	}

	@Override
	public void writeTop5UpBoToRedis(List<TradeModel> tradeList) {
		ShardedJedis shardedJedis = null;
		List<TopFiveBoMsg> top5UpBo = new ArrayList<>();
		RedisTopFiveBoModel redisModel = new RedisTopFiveBoModel();
		if (tradeList.size() > 0) {
			top5UpBo = processForTop5Bo(tradeList);
		} else {
			TopFiveBoMsg boMsg = new TopFiveBoMsg();
			top5UpBo.add(boMsg);
		}
		updatedBoPublisherServiceImpl.publishUpdatedTop5BO(top5UpBo, 1);
		try {
			shardedJedis = shardedJedisPool.getResource();
			redisModel.setBoList(top5UpBo);
			String redisBoStr = JSON.toJSONString(redisModel);
			LOGGER.info(">>>>>>>>>>> GOING TO WRITE TOP5BO UP TO REDIS <<<<<<<<<<<<<");
			shardedJedis.hset(CommonConstants.REDIS_KEY_RM_TOP5BO_UP, "Top5UpBo", redisBoStr);
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}
	}

	public void writeTop5DownBoToRedis(List<TradeModel> tradeList) {
		ShardedJedis shardedJedis = null;
		List<TopFiveBoMsg> top5DownBo = new ArrayList<>();
		RedisTopFiveBoModel redisModel = new RedisTopFiveBoModel();
		if (tradeList.size() > 0) {
			top5DownBo = processForTop5Bo(tradeList);
		} else {
			TopFiveBoMsg boMsg = new TopFiveBoMsg();
			top5DownBo.add(boMsg);
		}
		updatedBoPublisherServiceImpl.publishUpdatedTop5BO(top5DownBo, 0);
		try {
			shardedJedis = shardedJedisPool.getResource();
			redisModel.setBoList(top5DownBo);
			String redisBoStr = JSON.toJSONString(redisModel);
			LOGGER.info(">>>>>>>>>>> GOING TO WRITE TOP5BO DOWN TO REDIS <<<<<<<<<<<<<");
			shardedJedis.hset(CommonConstants.REDIS_KEY_RM_TOP5BO_DOWN, "Top5DownBo", redisBoStr);
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}
	}

	public List<TopFiveBoMsg> processForTop5Bo(List<TradeModel> tradeList) {

		List<TopFiveBoMsg> boList = new ArrayList<TopFiveBoMsg>();
		Map<Integer, List<TradeModel>> map = tradeList.stream().collect(Collectors.groupingBy(o -> o.getAssetid()));
		if (map.isEmpty())
			return boList;
		map.entrySet().forEach(p -> {
			try {
				List<TradeModel> tlist = p.getValue();
				int assetId = p.getKey();
				if (tlist.size() > 0) {
					TopFiveBoMsg bmodel = new TopFiveBoMsg();
					long sum = tlist.stream().mapToLong(o -> o.getAmount()).sum();
					bmodel.setAssetid(assetId);
					bmodel.setTotalvolume(sum);
					OptionalDouble opDouble = tlist.stream().mapToDouble(o -> o.getOpenprice()).average();
					Double average = opDouble.orElse(-1);
					bmodel.setAverageprice(average.longValue());
					OptionalLong opMin = tlist.stream().mapToLong(o -> o.getOpenprice()).min();
					long minValue = opMin.orElse(-1);
					bmodel.setLowestprice(minValue);
					Double averageVol = tlist.stream().mapToDouble(o -> o.getAmount()).average().orElse(-1);
					bmodel.setAveragevolume(averageVol.longValue());
					bmodel.setNooforders(tlist.size());
					bmodel.setScale(BoCommons.getScale(tlist));
					boList.add(bmodel);
				}
			} catch (Exception e) {
				LOGGER.error("ERROR FOLLOWED BY  : ", e);
				e.printStackTrace();
			}
		});
		Collections.sort(boList, Comparator.comparingLong(TopFiveBoMsg::getTotalvolume).reversed());

		if (boList.size() >= 5)
			return boList.subList(0, 5);
		else
			return boList;
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
