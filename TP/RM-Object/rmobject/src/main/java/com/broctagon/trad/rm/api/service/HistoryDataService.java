/**
 * 
 */
package com.broctagon.trad.rm.api.service;

import java.util.ArrayList;
import java.util.List;

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
import com.broctagon.trad.rm.obj.req.message.TraderHistoryReqMsg;
import com.broctagon.trad.rm.obj.res.message.TraderHistoryResMsg;
import com.broctagon.trad.rm.obj.serviceimpl.TraderHistoryServiceImpl;

/**
 * @author imdadullah
 *
 */
@Service
public class HistoryDataService {


	Logger LOGGER = Logger.getLogger(TraderHistoryServiceImpl.class);
	@Autowired
	CommonPublisher commonPublisher;
	@Autowired
	CloseTradeDao closeTradedoa;
	@Autowired
	OpenTradeDao openTradeDao;
	@Autowired
	OrderDao orderDao;

	public TraderHistoryResMsg tradersHistory(String reqMessage) {
		List<Order> tradeList;
		List<Order> tradeSubList;
		TraderHistoryResMsg historyRes = new TraderHistoryResMsg();
		TraderHistoryReqMsg req = JSON.parseObject(reqMessage, TraderHistoryReqMsg.class);
		LOGGER.info(">>>>>> GOING TO SEND HISTORY  <<<<<<<");
		LOGGER.info("REQUEST SG ID IS >>>>>>>>>>>>>" + req.getSgid());
				try {
					tradeList = getOpenClosedTradeList(req.getClientid());
					int size = tradeList.size();
					LOGGER.info("HISTORY LIST SIZE IS  >>>>>>>>>>>>>" + size);
					if(size >20){
						 tradeSubList = tradeList.subList(0, 20);
						 historyRes.setBody(tradeSubList);
						 historyRes.setTotal(tradeSubList.size());
					}
					else{
						historyRes.setBody(tradeList);
						historyRes.setTotal(tradeList.size());
					}
					historyRes.setClientid(req.getClientid());
					historyRes.setManagerid(req.getManagerid());
					historyRes.setReqid(req.getReqid());
					historyRes.setSgid(req.getSgid());
					historyRes.setSessionid(req.getSessionid());
					LOGGER.info(" <<<<<<<<<<<<<<< GOING TO SEND TRADERS HISTORY >>>>>>>>>>");
				} catch (Exception e) {
					LOGGER.error("ERROR MESSAGE FOLLOWED BY : ", e);
					e.printStackTrace();
				}
				return historyRes;
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
}
