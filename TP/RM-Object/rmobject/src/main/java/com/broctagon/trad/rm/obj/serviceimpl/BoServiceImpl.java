/**
 * 
 */
package com.broctagon.trad.rm.obj.serviceimpl;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.common.CommonUtil;
import com.broctagon.trad.rm.obj.service.BoService;
import com.broctagon.trad.rm.obj.service.CustomizeBoService;
import com.broctagon.trad.rm.obj.service.SymbolBoService;
import com.broctagon.trad.rm.obj.service.TopFiveBoService;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

/**
 * @author imdadullah
 *
 */
@Service
@Scope(value = "singleton")
public class BoServiceImpl implements BoService {
	private Logger LOGGER = Logger.getLogger(this.getClass());

	@Autowired private ShardedJedisPool shardedJedisPool;
	@Autowired private SymbolBoService symbolBoService;
	@Autowired private TopFiveBoService topFiveBOService;
	@Autowired private CustomizeBoService customizeBoService;
	@Autowired private UpdatedBoPublisherServiceImpl updateBoService;

	public List<TradeModel> openUpTradeList = new ArrayList<>();
	public List<TradeModel> openDownTradeList = new ArrayList<>();
	public List<TradeModel> closeTradeList = new ArrayList<>();
	public List<TradeModel> prevOpenUpTradeList = new ArrayList<>();
	public List<TradeModel> prevOpenDownTradeList = new ArrayList<>();
	public List<TradeModel> openUpListForUpdate = new ArrayList<>();
	public List<TradeModel> openDownListForUpdate = new ArrayList<>();

	@Override
	public void fetchOrderAnsAndProcess() {
		ShardedJedis shardedJedis = null;
		CommonUtil.checkServicePermission();
	//	LOGGER.info("<<<<<<<<<<<<< GOING TO PROCESS ORDERANS >>>>>>>>>>>");
		try {
			shardedJedis = shardedJedisPool.getResource();
			List<String> openUpOrder = shardedJedis.hvals(CommonConstants.REDIS_KEY_RM_HASH_OPENUP_ORDERANS);
			List<String> openDownOrder = shardedJedis.hvals(CommonConstants.REDIS_KEY_RM_HASH_OPENDOWN_ORDERANS);
			List<String> closedOrder = shardedJedis.hvals(CommonConstants.REDIS_KEY_RM_HASH_CLOSED_ORDERANS);

			openUpTradeList = topFiveBOService.getOpenUpTradeList(openUpOrder);
			openDownTradeList = topFiveBOService.getOpenDownTradeList(openDownOrder);
			closeTradeList = symbolBoService.getClosedTradeList(closedOrder);
			processOrderListForUpdatingBo();

			if (!prevOpenUpTradeList.equals(openUpTradeList)) {

				topFiveBOService.writeTop5UpBoToRedis(openUpTradeList);
				customizeBoService.writeCustomizedUpBoToRedis(openUpTradeList, openUpListForUpdate, closeTradeList);
			}
			
			if (!prevOpenDownTradeList.equals(openDownTradeList)) {
				topFiveBOService.writeTop5DownBoToRedis(openDownTradeList);
				customizeBoService.writeCustomizedDownBoToRedis(openDownTradeList, openDownListForUpdate,closeTradeList);
			}

			if (closeTradeList != null) {
				symbolBoService.writeSymbolBoToRedis(closeTradeList);
			}

			if ((!prevOpenUpTradeList.equals(openUpTradeList) || !prevOpenDownTradeList.equals(openDownTradeList))) {
				if (prevOpenUpTradeList.equals(openUpTradeList)) {
					updateBoService.updateOpenPositionTbl(openDownListForUpdate);
				} else if (prevOpenDownTradeList.equals(openDownTradeList)) {
					updateBoService.updateOpenPositionTbl(openUpListForUpdate);
				} else {
					List<TradeModel> openTradeList = Stream.concat(openUpListForUpdate.stream(), openDownListForUpdate.stream()).collect(Collectors.toList());
					updateBoService.updateOpenPositionTbl(openTradeList);
				}

			}
			
			if (closeTradeList != null) {
				updateBoService.updateClosedPositionTbl(closeTradeList);
			}
			prevOpenUpTradeList = openUpTradeList;
			prevOpenDownTradeList = openDownTradeList;

		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		} finally {
			if (shardedJedis != null) {
				shardedJedis.close();
			}
		}
	}

	public void processOrderListForUpdatingBo() {
		
		if (!prevOpenUpTradeList.equals(openUpTradeList)){
			List<TradeModel> openUpList = new ArrayList<>();
			openUpList.addAll(openUpTradeList);
			openUpList.removeAll(openUpListForUpdate);
			openUpListForUpdate.clear();
			openUpListForUpdate.addAll(openUpList);
		}
		if(!prevOpenDownTradeList.equals(openDownTradeList)){
			List<TradeModel> openDownList = new ArrayList<>();
			openDownList.addAll(openDownTradeList);
			openDownList.removeAll(openDownListForUpdate);
			openDownListForUpdate.clear();
			openDownListForUpdate.addAll(openDownList);
		}
		
	}
	
}
