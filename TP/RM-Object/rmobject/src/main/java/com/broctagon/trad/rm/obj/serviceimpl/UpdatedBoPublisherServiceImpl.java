/**
 * 
 */
package com.broctagon.trad.rm.obj.serviceimpl;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.db.dao.CloseTradeDao;
import com.broctagon.trad.rm.db.dao.OpenTradeDao;
import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.common.CommonPublisher;
import com.broctagon.trad.rm.obj.message.CustomBoMsg;
import com.broctagon.trad.rm.obj.message.SymbolBoMsg;
import com.broctagon.trad.rm.obj.message.TopFiveBoMsg;
import com.broctagon.trad.rm.obj.model.TopFiveUpdateBoModel;
import com.broctagon.trad.rm.obj.req.message.CustomBoReqMsg;
import com.broctagon.trad.rm.obj.req.message.SymbolBoReqMsg;
import com.broctagon.trad.rm.obj.req.message.TopFiveBoReqMsg;
import com.broctagon.trad.rm.obj.req.serviceimpl.BoReqServiceImpl;


/**
 * @author imdadullah
 *
 */
@Service
public class UpdatedBoPublisherServiceImpl {
	Logger LOGGER = Logger.getLogger(this.getClass());
	@Autowired
	private CommonPublisher commonPublisher;
	@Autowired
	private OpenTradeDao openTradeDao;
	@Autowired
	private CloseTradeDao closeTradeDao;
	@Autowired
	private BoReqServiceImpl boReqServiceImpl;
	public List<TradeModel> tradListForUpdate = new ArrayList<>();

	public void updateOpenPositionTbl(List<TradeModel> openTradeListForDB) {

		if (!openTradeListForDB.isEmpty()) {
			openTradeDao.insertOpenTrades(openTradeListForDB);
		}
	}

	public void updateClosedPositionTbl(List<TradeModel> closedPositionList) {
		List<String> orderIdList = null;
		if (closedPositionList != null) {
			Map<String, List<TradeModel>> map = closedPositionList.stream()
					.collect(Collectors.groupingBy(o -> o.getOrderid()));
			orderIdList = map.keySet().stream().collect(Collectors.toList());
		}
		if (orderIdList != null) {
			openTradeDao.deleteOpenTrades(orderIdList);
			closeTradeDao.insertClosePositions(closedPositionList);
		}
	}

	public void publishUpdatedTop5BO(List<TopFiveBoMsg> top5UpBo, int direction) {
		TopFiveUpdateBoModel updateBoModel = new TopFiveUpdateBoModel();
		Map<Integer, TopFiveBoReqMsg> topBoReqMap = boReqServiceImpl.getTopBoReqMap();
		if (topBoReqMap.size() == 0)
			return;
		updateBoModel.setDirection(direction);
		updateBoModel.setModels(top5UpBo);
		String top5BoStr = JSON.toJSONString(updateBoModel);
		LOGGER.info("<<<<<<<<<<< GOING TO PUBLISH UPDATED TOP5BO >>>>>>>>>>>>" + top5BoStr);
		commonPublisher.publishTop5UpdatedBo(top5BoStr);
	}

	public void publishUpdatedCustomBO(Map<Integer, CustomBoMsg> customBoMap, List<TradeModel> tradList,
			List<TradeModel> closeTradeList, int direction) {
		List<CustomBoReqMsg> reqList;
		List<Integer> symbol;
		List<Integer> reqSymbol;
		CustomBoMsg customBoMsg = null;
		Map<Integer, List<TradeModel>> openClosedSymbol = null;
		Map<Integer, List<CustomBoReqMsg>> reqMap = boReqServiceImpl.getCustomReqMap();
		List<TradeModel> closedUpList = new ArrayList<>();
		List<TradeModel> closedDownList = new ArrayList<>();
		if (tradList != null) {
			tradListForUpdate.addAll(tradList);
		}
		if (closeTradeList != null) {
			closedUpList = closeTradeList.stream().filter(o -> o.getDirection() == 1).collect(Collectors.toList());
			closedDownList = closeTradeList.stream().filter(o -> o.getDirection() == 0).collect(Collectors.toList());
		}

		if (closedUpList.size() > 0) {
			tradListForUpdate.addAll(closedUpList);
		}
		if (closedDownList.size() > 0) {
			tradListForUpdate.addAll(closedDownList);
		}
		openClosedSymbol = tradListForUpdate.stream().collect(Collectors.groupingBy(o -> o.getAssetid()));
		reqSymbol = reqMap.keySet().stream().collect(Collectors.toList());
		symbol = openClosedSymbol.keySet().stream().collect(Collectors.toList());
		symbol.retainAll(reqSymbol);
		for (Integer symbolId : symbol) {
			reqList = reqMap.get(symbolId).stream().filter(o -> o.getDirection() == direction)
					.collect(Collectors.toList());
			if (customBoMap != null) {
				customBoMsg = customBoMap.get(symbolId);
				if (customBoMsg == null) {
					customBoMsg = new CustomBoMsg();
					customBoMsg.setAssetid(symbolId);
				}

			} else {
				customBoMsg = new CustomBoMsg();
				customBoMsg.setAssetid(symbolId);
			}
			if (!reqList.isEmpty()) {
				customBoMsg.setDirection(direction);
				String customBoStr = JSON.toJSONString(customBoMsg);
				LOGGER.info("<<<<<<<<<<< GOING TO PUBLISH UPDATED CUSTOMBO >>>>>>>>>>>>" + customBoStr);
				commonPublisher.publishCustomizedUpdatedBo(customBoStr);
			}
		}
		tradListForUpdate.clear();
	}

	public void publishUpdatedSymbolBO(Map<Integer, SymbolBoMsg> addedSymbolBoMap, List<TradeModel> closeTradeList) {
		List<Integer> symbol = new ArrayList<>();
		Map<Integer, List<TradeModel>> closedSymbolMap = null;
		SymbolBoMsg symbolBoMessage = null;
		List<SymbolBoReqMsg> reqSymbolBoMsgs = boReqServiceImpl.getSymbolReqMap().values().stream()
				.collect(Collectors.toList());
		if (reqSymbolBoMsgs.isEmpty())
			return;
		closedSymbolMap = closeTradeList.stream().collect(Collectors.groupingBy(o -> o.getAssetid()));
		symbol = closedSymbolMap.keySet().stream().collect(Collectors.toList());
		for (Integer symbolId : symbol) {
			symbolBoMessage = addedSymbolBoMap.get(symbolId);
			if (symbolBoMessage == null) {
				symbolBoMessage = new SymbolBoMsg();
				symbolBoMessage.setAssetid(symbolId);
			}
			String symbolBoStr = JSON.toJSONString(symbolBoMessage);
			LOGGER.info("<<<<<<<<<<< GOING TO PUBLISH UPDATED SYMBOLMBO >>>>>>>>>>>>" + symbolBoStr);
			commonPublisher.publishSymbolUpdatedBo(symbolBoStr);
			// positionService.updateTraderForClosePosition();
		}
	}
}