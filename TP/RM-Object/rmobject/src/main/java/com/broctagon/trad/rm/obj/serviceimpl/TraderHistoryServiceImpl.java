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
import com.broctagon.trad.rm.db.dao.OrderDao;
import com.broctagon.trad.rm.db.model.Order;
import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.common.CommonPublisher;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.message.UpdateClosedPositionMsg;
import com.broctagon.trad.rm.obj.model.UpdateClosedPositionModel;
import com.broctagon.trad.rm.obj.req.message.TraderHistoryReqMsg;
import com.broctagon.trad.rm.obj.res.message.TraderHistoryResMsg;
import com.broctagon.trad.rm.obj.service.TraderHistoryService;
import com.broctagon.tradstnode.rabbit.message.Message;

/**
 * @author imdadullah
 *
 */
@Service
public class TraderHistoryServiceImpl implements TraderHistoryService {

	Logger LOGGER = Logger.getLogger(TraderHistoryServiceImpl.class);
	@Autowired
	CommonPublisher commonPublisher;
	@Autowired
	CloseTradeDao closeTradedoa;
	@Autowired
	OpenTradeDao openTradeDao;
	@Autowired
	OrderDao orderDao;

	public void tradersHistory(Message message, int reqTag) {
		List<Order> tradeList;
		String messageString = message.getMessageString();
		TraderHistoryReqMsg req = JSON.parseObject(messageString, TraderHistoryReqMsg.class);
		LOGGER.info(">>>>>> GOING TO SEND HISTORY  <<<<<<<");
		LOGGER.info("REQUEST SG ID IS >>>>>>>>>>>>>" + req.getSgid());
		tradeList = getOpenClosedTradeList(req.getClientid());
		int size = tradeList.size();
		LOGGER.info("HISTORY LIST SIZE IS  >>>>>>>>>>>>>" + size);
		if (size > 14) {
			int loopNo = (size / 14);
			for (int i = 0; i < loopNo; i++) {
				try {
					List<Order> tradeSubList = tradeList.subList(0, 14);
					TraderHistoryResMsg historyRes = new TraderHistoryResMsg();
					historyRes.setClientid(req.getClientid());
					historyRes.setManagerid(req.getManagerid());
					historyRes.setReqid(req.getReqid());
					historyRes.setBody(tradeSubList);
					historyRes.setTotal(tradeSubList.size());
					historyRes.setSgid(req.getSgid());
					historyRes.setSessionid(req.getSessionid());
					LOGGER.info(" <<<<<<<<<<<<<<< GOING TO SEND TRADERS HISTORY >>>>>>>>>>");
					commonPublisher.publishHistoryToSG(historyRes, req.getSgid());
					tradeSubList.subList(0, 14).clear();
				} catch (Exception e) {
					LOGGER.error("ERROR MESSAGE FOLLOWED BY : ", e);
					e.printStackTrace();
				}
			}

			TraderHistoryResMsg historyRes = new TraderHistoryResMsg();
			historyRes.setClientid(req.getClientid());
			historyRes.setManagerid(req.getManagerid());
			historyRes.setReqid(req.getReqid());
			historyRes.setBody(tradeList);
			historyRes.setTotal(tradeList.size());
			historyRes.setSgid(req.getSgid());
			historyRes.setSessionid(req.getSessionid());
			LOGGER.info(" <<<<<<<<<<<<<<< GOING TO SEND TRADERS HISTORY >>>>>>>>>>");
			commonPublisher.publishHistoryToSG(historyRes, req.getSgid());

		} else {
			if (size != 0) {
				TraderHistoryResMsg historyRes = new TraderHistoryResMsg();
				historyRes.setClientid(req.getClientid());
				historyRes.setManagerid(req.getManagerid());
				historyRes.setReqid(req.getReqid());
				historyRes.setBody(tradeList);
				historyRes.setTotal(tradeList.size());
				historyRes.setSgid(req.getSgid());
				historyRes.setSessionid(req.getSessionid());
				LOGGER.info(" <<<<<<<<<<<<<<< GOING TO SEND TRADERS HISTORY >>>>>>>>>>");
				commonPublisher.publishHistoryToSG(historyRes, req.getSgid());
			}

		}
	}

	public List<Order> getOpenClosedTradeList(int clientId) {
		List<Order> tradeList = null;
		List<TradeModel> openTradeList = null;
		try {
			if (clientId > 0) {
				tradeList = orderDao.queryLatest30ClosedAndBalanceOrdersByClientId(clientId);
				openTradeList = openTradeDao.selectOpenTradeByClientid(clientId);
				if (openTradeList != null) {
					tradeList.addAll(getOrder(openTradeList));
				}

			} else if (clientId == 0) {
				// tradeList =
				// orderDao.queryLatest30ClosedAndBalanceOrdersByClientId(clientId);
			}
			return tradeList;
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
			return null;
		}
	}

	public List<Order> getOrder(List<TradeModel> openTradeList) {
		List<Order> orderList = new ArrayList<>();
		for (TradeModel model : openTradeList) {
			Order order = new Order();
			order.setAmount((int) model.getAmount());
			order.setAssetid(model.getAssetid());
			order.setClientid(model.getClientid());
			order.setCloseprice((int) model.getCloseprice());
			order.setComment(model.getComment());
			order.setDirection(model.getDirection());
			order.setExpiryperiod(model.getExpiryperiod());
			order.setExpirytime(model.getExpirytime());
			order.setGroupid(model.getGroupid());
			order.setOpenprice((int) model.getOpenprice());
			order.setOpentime(model.getOpentime());
			order.setOrderid(model.getOrderid());
			order.setOrderstatus(model.getOrderstatus());
			order.setPayout(model.getPayout());
			order.setReqid(model.getReqid());
			order.setSgid(model.getSgid());
			order.setTeid(model.getTeid());
			order.setType(model.getType());
			orderList.add(order);
		}
		return orderList;

	}

	public void updateTraderHistory(List<TradeModel> closeTradeList) {

		if (closeTradeList == null)
			return;
		String routingKeyName = CommonConstants.GET_UPDATECLOSEDTRADE_RES;
		int tag = CommonConstants.GET_UPDATECLOSEDTRADE_TAG;
		UpdateClosedPositionModel updateAns = new UpdateClosedPositionModel();
		List<UpdateClosedPositionMsg> msgList = new ArrayList<UpdateClosedPositionMsg>();
		Map<String, List<TradeModel>> closedPositionMap = closeTradeList.stream()
				.collect(Collectors.groupingBy(o -> o.getOrderid()));
		List<String> orderIdList = closedPositionMap.keySet().stream().collect(Collectors.toList());
		for (String orderId : orderIdList) {
			UpdateClosedPositionMsg closePositionMsg = new UpdateClosedPositionMsg();
			closePositionMsg.setOrderid(orderId);
			msgList.add(closePositionMsg);
		}
		updateAns.setBody(msgList);
		String ansMsg = JSON.toJSONString(updateAns);
		commonPublisher.pubUpdatedPositionToMGSG(routingKeyName, tag, ansMsg);

	}

}
