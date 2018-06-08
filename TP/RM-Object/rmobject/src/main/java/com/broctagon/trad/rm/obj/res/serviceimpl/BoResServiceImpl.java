/**
 * 
 */
package com.broctagon.trad.rm.obj.res.serviceimpl;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.message.CustomBoMsg;
import com.broctagon.trad.rm.obj.message.SymbolBoMsg;
import com.broctagon.trad.rm.obj.model.RedisCustomizedBoModel;
import com.broctagon.trad.rm.obj.model.RedisSymbolBoModel;
import com.broctagon.trad.rm.obj.model.RedisTopFiveBoModel;
import com.broctagon.trad.rm.obj.req.message.CustomBoReqMsg;
import com.broctagon.trad.rm.obj.req.message.SymbolBoReqMsg;
import com.broctagon.trad.rm.obj.req.message.TopFiveBoReqMsg;
import com.broctagon.trad.rm.obj.res.message.CustomBoResMsg;
import com.broctagon.trad.rm.obj.res.message.SymbolBoResMsg;
import com.broctagon.trad.rm.obj.res.message.TopFiveBoResMsg;
import com.broctagon.trad.rm.obj.service.BoResService;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;


/**
 * @author imdadullah
 *
 */
@Service
public class BoResServiceImpl implements BoResService {
	private Logger LOGGER = Logger.getLogger(this.getClass());
	@Autowired
	private ShardedJedisPool shardedJedisPool;

	public String getTop5BoUpRes(TopFiveBoReqMsg req) {
		ShardedJedis shardedJedis = null;
		RedisTopFiveBoModel boModel = null;
		String boResMsgStr = null;
		String redisBoMsgStr = null;
		TopFiveBoResMsg resMsg = new TopFiveBoResMsg();
		try {
			shardedJedis = shardedJedisPool.getResource();

			redisBoMsgStr = shardedJedis.hget(CommonConstants.REDIS_KEY_RM_TOP5BO_UP, "Top5UpBo");

			boModel = JSON.parseObject(redisBoMsgStr, RedisTopFiveBoModel.class);
			int sgid = req.getSgid();
			String session = req.getSession();
			if (boModel != null) {
				resMsg.setDirection(req.getDirection());
				resMsg.setReqid(req.getReqid());
				resMsg.setSgid(sgid);
				resMsg.setManagerid(req.getManagerid());
				resMsg.setSession(session);
				resMsg.setModels(boModel.getBoList());
				boResMsgStr = JSON.toJSONString(resMsg);
			}
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}
		return boResMsgStr;
	}

	public String getTop5BoDownRes(TopFiveBoReqMsg req) {
		ShardedJedis shardedJedis = null;
		RedisTopFiveBoModel boModel = null;
		String boResMsgStr = null;
		String redisBoMsgStr = null;
		TopFiveBoResMsg resMsg = new TopFiveBoResMsg();
		try {
			shardedJedis = shardedJedisPool.getResource();

			redisBoMsgStr = shardedJedis.hget(CommonConstants.REDIS_KEY_RM_TOP5BO_DOWN, "Top5DownBo");

			boModel = JSON.parseObject(redisBoMsgStr, RedisTopFiveBoModel.class);
			int sgid = req.getSgid();
			String session = req.getSession();
			if (boModel != null) {
				resMsg.setDirection(req.getDirection());
				resMsg.setReqid(req.getReqid());
				resMsg.setSgid(sgid);
				resMsg.setManagerid(req.getManagerid());
				resMsg.setSession(session);
				resMsg.setModels(boModel.getBoList());
				boResMsgStr = JSON.toJSONString(resMsg);
			}
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}
		return boResMsgStr;
	}

	public String getCustomBoRes(CustomBoReqMsg req) {
		ShardedJedis shardedJedis = null;
		RedisCustomizedBoModel boModel = null;
		String boResMsgStr = null;
		String redisBoMsgStr = null;
		CustomBoMsg customBoMsg = null;
		CustomBoResMsg resMsg = new CustomBoResMsg();
		try {
			shardedJedis = shardedJedisPool.getResource();
			if (req.getDirection() == 1) {
				redisBoMsgStr = shardedJedis.hget(CommonConstants.REDIS_KEY_RM_CUSTOMBO_UP, "CustomUpBo");
			} else if (req.getDirection() == 0) {
				redisBoMsgStr = shardedJedis.hget(CommonConstants.REDIS_KEY_RM_CUSTOMBO_DOWN, "CustomDownBo");
			}
			if (redisBoMsgStr == null)
				return null;
			boModel = JSON.parseObject(redisBoMsgStr, RedisCustomizedBoModel.class);
			Map<Integer, CustomBoMsg> boMap = boModel.getBoMap();

			if (boMap != null) {
				customBoMsg = boMap.get(req.getAssetid());
				List<CustomBoMsg> customMsgList = new ArrayList<>();
				customMsgList.add(customBoMsg);
				resMsg.setDirection(req.getDirection());
				resMsg.setReqid(req.getReqid());
				resMsg.setSgid(req.getSgid());
				resMsg.setManagerid(req.getManagerid());
				resMsg.setSession(req.getSession());
				resMsg.setModel(customMsgList);
				boResMsgStr = JSON.toJSONString(resMsg);
			}
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}
		return boResMsgStr;
	}

	public String getSymbolBoRes(SymbolBoReqMsg req) {
		ShardedJedis shardedJedis = null;
		RedisSymbolBoModel boModel = null;
		String boResMsgStr = null;
		String redisBoMsgStr = null;
		SymbolBoResMsg resMsg = new SymbolBoResMsg();
		try {
			shardedJedis = shardedJedisPool.getResource();
			redisBoMsgStr = shardedJedis.hget(CommonConstants.REDIS_KEY_RM_SYMBOLBO, "SymbolBo");
			if (redisBoMsgStr == null)
				return null;
			boModel = JSON.parseObject(redisBoMsgStr, RedisSymbolBoModel.class);
			Map<Integer, SymbolBoMsg> boMap = boModel.getBoMap();
			List<SymbolBoMsg> symbolBoMsgList = boMap.values().stream().collect(Collectors.toList());
			int sgid = req.getSgid();
			String session = req.getSession();
			if (boModel != null) {
				resMsg.setReqid(req.getReqid());
				resMsg.setSgid(sgid);
				resMsg.setManagerid(req.getManagerid());
				resMsg.setSession(session);
				resMsg.setSymbolboans(symbolBoMsgList);
				boResMsgStr = JSON.toJSONString(resMsg);
			}
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}
		return boResMsgStr;
	}
}
